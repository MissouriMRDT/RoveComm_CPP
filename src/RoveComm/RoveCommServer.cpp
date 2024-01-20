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
#include "../RoveCommLogging.h"
#include "RoveCommEthernetTcp.h"
#include "RoveCommEthernetUdp.h"

RoveCommServerManager RoveComm = RoveCommServerManager();

void RoveCommServerManager::Init()
{
    OpenServerOnPort(rovecomm::General::ETHERNET_TCP_PORT, TCP);
    OpenServerOnPort(rovecomm::General::ETHERNET_UDP_PORT, UDP);

    bStopThread       = false;
    s_thNetworkThread = std::thread(
        []()
        {
            while (!bStopThread)
            {
                _loop_func();
            }
        });
}

void RoveCommServerManager::Shutdown()
{
    bStopThread = true;
    s_thNetworkThread.join();    // block until _loop_func() finishes
    for (auto& [protocol, server] : s_mServers)
    {
        server->Shutdown();
        delete server;
    }
    s_mServers.clear();
}

void RoveCommServerManager::_loop_func()
{
    _read_all_to_queue();
    // invoke callbacks
}

void RoveCommServerManager::OpenServerOnPort(RoveCommPort port, RoveCommProtocol protocol)
{
    const std::lock_guard lock(s_muQueueMutex);
    if (s_mServers.contains(protocol))
    {
        LOG_ERROR(logging::g_qSharedLogger, "Server already open with that protocol.");
    }
    switch (protocol)
    {
        case TCP:
        {
            (s_mServers[protocol] = new RoveCommEthernetTcp(port))->Init();
            break;
        }
        case UDP:
        {
            (s_mServers[protocol] = new RoveCommEthernetUdp(port))->Init();
            break;
        }
        default:
        {
            LOG_ERROR(logging::g_qSharedLogger, "Tried to open server with an unregistered protocol.");
        }
    }
}

int RoveCommServerManager::Write(RoveCommPacket& packet, RoveCommProtocol protocol)
{
    const std::lock_guard lock(s_muQueueMutex);
    int nSent = 0;
    for (auto& [serverProtocol, server] : s_mServers)
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
    const std::lock_guard lock(s_muQueueMutex);
    int nSent = 0;
    for (auto& [serverProtocol, server] : s_mServers)
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
    for (auto& [serverProtocol, server] : s_mServers)
    {
        for (auto& packet : server->Read())
        {
            auto nDataId = packet.GetDataId();
            if (s_mCallbacks.contains(nDataId))
            {
                auto callback = s_mCallbacks.at(nDataId);
                callback.fInvoke(packet);
                if (callback.bRemoveFromQueue)
                    continue;
            }
            s_dqPacketQueue.push_back(packet);
        }
    }
}

std::optional<const RoveCommPacket> RoveCommServerManager::Next()
{
    const std::lock_guard lock(s_muQueueMutex);
    if (s_dqPacketQueue.empty())
    {
        return std::nullopt;
    }
    else
    {
        auto packet = s_dqPacketQueue.front();    // copy
        s_dqPacketQueue.pop_front();              // remove
        return std::optional<const RoveCommPacket>{packet};
    }
}

void RoveCommServerManager::SetCallback(RoveCommDataId unId, std::function<void(RoveCommPacket)> fCallback, bool bRemoveFromQueue)
{
    const std::lock_guard lock(s_muQueueMutex);
    s_mCallbacks[unId] = {fCallback, bRemoveFromQueue};
}

void RoveCommServerManager::ClearCallback(RoveCommDataId unId)
{
    const std::lock_guard lock(s_muQueueMutex);
    s_mCallbacks.erase(unId);
}
