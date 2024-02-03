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

    m_thNetworkThread = std::thread(    // init network thread
        [&]()
        {
            while (!m_bStopThread)
            {
                _loop_func();
            }
        });
}

void RoveCommServerManager::Shutdown()
{
    LOG_INFO(logging::g_qSharedLogger, "Closing RoveComm...");
    m_bStopThread = true;
    m_thNetworkThread.join();    // block until _loop_func() finishes
    for (auto& [protocol, server] : m_mServers)
    {
        server->Shutdown();
        delete server;
    }
    m_mServers.clear();
}

void RoveCommServerManager::_loop_func()
{
    {
        const std::lock_guard lock(m_muQueueMutex);
        for (auto& [protocol, server] : m_mServers)
        {
            server->OnRoveCommUpdate();
        }
    }
    _read_all_to_queue();
}

bool RoveCommServerManager::OpenServerOnPort(uint16_t port, RoveCommProtocol protocol)
{
    const std::lock_guard lock(m_muQueueMutex);
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

int RoveCommServerManager::Write(const RoveCommPacket& packet, RoveCommProtocol protocol)
{
    const std::lock_guard lock(m_muQueueMutex);
    int nSent = 0;
    for (auto& [serverProtocol, server] : m_mServers)
    {
        if (serverProtocol & protocol)
        {
            nSent += server->Write(packet);
        }
    }
    return nSent;
}

int RoveCommServerManager::SendTo(const RoveCommPacket& packet, RoveCommAddress address, RoveCommProtocol protocol)
{
    const std::lock_guard lock(m_muQueueMutex);
    int nSent = 0;
    for (auto& [serverProtocol, server] : m_mServers)
    {
        if (serverProtocol & protocol)
        {
            nSent += server->SendTo(packet, address);
        }
    }
    return nSent;
}

void RoveCommServerManager::_read_all_to_queue()
{
    // const std::lock_guard lock(m_muQueueMutex);
    for (auto& [serverProtocol, server] : m_mServers)
    {
        for (auto& packet : server->Read())    // for each incoming packet in all servers
        {
            auto nDataId = packet.GetDataId();
            if (m_mCallbacks.contains(nDataId))    // check if the dataId matches some callback and invoke it
            {
                auto callback = m_mCallbacks.at(nDataId);
                callback.fInvoke(packet);    // TODO: use AutonomyThread to invoke callbacks asynchronously
                if (callback.bRemoveFromQueue)
                    continue;                // do not propogate packet to queue
            }
            m_dqPacketQueue.push_back(packet);
        }
    }
}

std::optional<const RoveCommPacket> RoveCommServerManager::NextPacket()
{
    const std::lock_guard lock(m_muQueueMutex);
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

void RoveCommServerManager::SetCallback(uint16_t unId, std::function<void(RoveCommPacket)> fCallback, bool bRemoveFromQueue)
{
    const std::lock_guard lock(m_muQueueMutex);
    m_mCallbacks[unId] = {fCallback, bRemoveFromQueue};
}

void RoveCommServerManager::ClearCallback(uint16_t unId)
{
    const std::lock_guard lock(m_muQueueMutex);
    m_mCallbacks.erase(unId);
}
