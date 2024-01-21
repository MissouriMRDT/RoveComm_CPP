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

    m_thNetworkThread = std::thread(
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

bool RoveCommServerManager::OpenServerOnPort(RoveCommPort port, RoveCommProtocol protocol)
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

int RoveCommServerManager::Write(RoveCommPacket& packet, RoveCommProtocol protocol)
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

int RoveCommServerManager::SendTo(RoveCommPacket& packet, RoveCommAddress address, RoveCommProtocol protocol)
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
    const std::lock_guard lock(m_muQueueMutex);
    for (auto& [serverProtocol, server] : m_mServers)
    {
        for (auto& packet : server->Read())
        {
            auto nDataId = packet.GetDataId();
            if (m_mCallbacks.contains(nDataId))
            {
                auto callback = m_mCallbacks.at(nDataId);
                callback.fInvoke(packet);
                if (callback.bRemoveFromQueue)
                    continue;
            }
            m_dqPacketQueue.push_back(packet);
        }
    }
}

std::optional<const RoveCommPacket> RoveCommServerManager::Next()
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

void RoveCommServerManager::SetCallback(RoveCommDataId unId, std::function<void(RoveCommPacket)> fCallback, bool bRemoveFromQueue)
{
    const std::lock_guard lock(m_muQueueMutex);
    const std::lock_guard lock(m_muQueueMutex);
    m_mCallbacks[unId] = {fCallback, bRemoveFromQueue};
}

void RoveCommServerManager::ClearCallback(RoveCommDataId unId)
{
    const std::lock_guard lock(m_muQueueMutex);
    const std::lock_guard lock(m_muQueueMutex);
    m_mCallbacks.erase(unId);
}
