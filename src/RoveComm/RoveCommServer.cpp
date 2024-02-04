/******************************************************************************
 * @brief Network thread management
 *
 * @file RoveCommServer.cpp
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-13
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#include "RoveCommServer.h"
#include "LogHack.h"
#include "RoveCommEthernetTcp.h"
#include "RoveCommEthernetUdp.h"

bool RoveCommServerManager::Init()
{
    LOG_INFO(logging::g_qSharedLogger, "Initializing RoveComm...");

    if (!(OpenServerOnPort(rovecomm::General::ETHERNET_TCP_PORT, TCP) &&    // init TCP
          OpenServerOnPort(rovecomm::General::ETHERNET_UDP_PORT, UDP)))     // init UDP
        return false;

    this->Start();    // AutonomyThread

    return true;
}

void RoveCommServerManager::Shutdown()
{
    LOG_INFO(logging::g_qSharedLogger, "Closing RoveComm...");
    this->Join();    // AutonomyThread
    for (auto& [protocol, server] : m_mServers)
    {
        server->Shutdown();
        delete server;
    }
    m_mServers.clear();
}

void RoveCommServerManager::ThreadedContinuousCode()    // AutonomyThread
{
    for (auto& [protocol, server] : m_mServers)
    {
        std::unique_lock lkServerState(m_muServerStateMutex);
        server->OnRoveCommUpdate();
        lkServerState.unlock();
    }
    // read all incoming packets to queue
    int nCallbackCount = 0;
    for (auto& [serverProtocol, server] : m_mServers)
    {
        std::unique_lock lkServerState(m_muServerStateMutex);
        auto vPackets = server->Read();
        lkServerState.unlock();
        for (auto& packet : vPackets)    // for each incoming packet in all servers
        {
            auto nDataId = packet.GetDataId();
            std::shared_lock lkCallbackList(m_muCallbackListMutex);
            if (m_mCallbacks.contains(nDataId))    // check if the dataId matches some callback and invoke it
            {
                nCallbackCount++;
                auto callback = m_mCallbacks.at(nDataId);
                lkCallbackList.unlock();
                std::unique_lock lkCallbackInvoke(m_muCallbackInvokeMutex);
                m_dqCallbackInvokeQueue.emplace_back(std::bind(callback.fInvoke, std::ref(packet)));
                lkCallbackInvoke.unlock();
                if (callback.bRemoveFromQueue)
                    continue;    // do not propogate packet to queue
            }
            std::lock_guard lkPacket(m_muPacketQueueMutex);
            m_dqPacketQueue.push_back(packet);
        }
    }
    this->RunDetachedPool(nCallbackCount, s_nMaxCallbackThreads);    // AutonomyThread
}

void RoveCommServerManager::PooledLinearCode()    // AutonomyThread
{
    std::unique_lock lkCallbackInvoke(m_muCallbackInvokeMutex);
    if (!m_dqCallbackInvokeQueue.empty())
    {
        auto fInvoke = m_dqCallbackInvokeQueue.front();
        m_dqCallbackInvokeQueue.pop_front();
        lkCallbackInvoke.unlock();
        fInvoke();
    }
}

bool RoveCommServerManager::OpenServerOnPort(uint16_t port, RoveCommProtocol protocol)
{
    if (m_mServers.contains(protocol))
    {
        LOG_ERROR(logging::g_qSharedLogger, "Server already open with that protocol.");
        return false;
    }
    switch (protocol)
    {
        case TCP:
        {
            RoveCommEthernetTcp* server = new RoveCommEthernetTcp(port);
            if (server->Init())
            {
                m_mServers[protocol] = server;
                return true;
            }
            break;
        }
        case UDP:
        {
            RoveCommEthernetUdp* server = new RoveCommEthernetUdp(port);
            if (server->Init())
            {
                m_mServers[protocol] = server;
                return true;
            }
            break;
        }
        default:
        {
            LOG_ERROR(logging::g_qSharedLogger, "Tried to open server with an unregistered protocol.");
        }
    }
    return false;
}

void RoveCommServerManager::Write(const RoveCommPacket& packet, RoveCommProtocol protocol)
{
    std::lock_guard lock(m_muCommandQueueMutex);
    m_dqCommandQueue.emplace_back(std::bind(&RoveCommServerManager::_write, this, std::ref(packet), protocol));
}

void RoveCommServerManager::_write(const RoveCommPacket& packet, RoveCommProtocol protocol)
{
    int nSent = 0;
    for (auto& [serverProtocol, server] : m_mServers)
    {
        if (serverProtocol & protocol)
        {
            std::unique_lock lkServerState(m_muServerStateMutex);
            nSent += server->Write(packet);
            lkServerState.unlock();
        }
    }
    // return nSent;
}

void RoveCommServerManager::SendTo(const RoveCommPacket& packet, const RoveCommAddress& address, RoveCommProtocol protocol)
{
    std::lock_guard lock(m_muCommandQueueMutex);
    m_dqCommandQueue.emplace_back(std::bind(&RoveCommServerManager::_send_to, this, std::ref(packet), std::ref(address), protocol));
}

void RoveCommServerManager::_send_to(const RoveCommPacket& packet, const RoveCommAddress& address, RoveCommProtocol protocol)
{
    int nSent = 0;
    for (auto& [serverProtocol, server] : m_mServers)
    {
        if (serverProtocol & protocol)
        {
            std::unique_lock lkServerState(m_muServerStateMutex);
            nSent += server->SendTo(packet, address);
            lkServerState.unlock();
        }
    }
    // return nSent;
}

std::optional<const RoveCommPacket> RoveCommServerManager::NextPacket()
{
    std::lock_guard lock(m_muPacketQueueMutex);
    if (m_dqPacketQueue.empty())
    {
        return std::nullopt;
    }
    else
    {
        auto packet = m_dqPacketQueue.front();    // copy
        m_dqPacketQueue.pop_front();              // remove
        return std::optional<const RoveCommPacket>{packet};
    }
}

void RoveCommServerManager::SubscribeTo(const RoveCommAddress& address, RoveCommProtocol protocol)
{
    std::lock_guard lock(m_muCommandQueueMutex);
    m_dqCommandQueue.emplace_back(std::bind(&RoveCommServerManager::_subscribe_to, this, std::ref(address), protocol));
}

void RoveCommServerManager::_subscribe_to(const RoveCommAddress& address, RoveCommProtocol protocol)
{
    for (auto& [serverProtocol, server] : m_mServers)
    {
        if (serverProtocol & protocol)
        {
            std::unique_lock lkServerState(m_muServerStateMutex);
            server->SubscribeTo(address);
            lkServerState.unlock();
        }
    }
}

// std::shared_future<RoveCommPacket> RoveCommServerManager::Fetch(RoveCommDataId unId          = rovecomm::System::ANY_DATA_ID,
//                                                                 RoveCommAddress address      = RoveCommAddress::ANY_ADDRESS,
//                                                                 unsigned long long ulTimeout = 0)
// {
//     Fetch([&](const RoveCommPacket& packet, const RoveCommAddress& address) -> bool { return true; }, ulTimeout);
// }

// std::shared_future<RoveCommPacket> RoveCommServerManager::Fetch(std::function<bool(const RoveCommPacket&, const RoveCommAddress&)> fFilter,
//                                                                 unsigned long long ulTimeout = 0)
// {
//     const std::lock_guard lock(m_muQueueMutex);
//     RoveCommFetchRequest request{.fPredicate = fFilter,
//                                  .pmPromise  = std::promise<RoveCommPacket>{},
//                                  .tmCreated  = std::chrono::system_clock::now(),
//                                  .tmTimeout  = std::chrono::milliseconds(ulTimeout)};
//     m_dqRequestQueue.push_back(request);
//     return std::shared_future<RoveCommPacket>(request.pmPromise.get_future());
// }

void RoveCommServerManager::SetCallback(uint16_t unId, std::function<void(const RoveCommPacket&)> fCallback, bool bRemoveFromQueue)
{
    const std::unique_lock lock(m_muCallbackListMutex);
    m_mCallbacks[unId] = {fCallback, bRemoveFromQueue};
}

void RoveCommServerManager::ClearCallback(uint16_t unId)
{
    const std::unique_lock lock(m_muCallbackListMutex);
    m_mCallbacks.erase(unId);
}
