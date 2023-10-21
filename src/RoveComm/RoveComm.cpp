/******************************************************************************
 * @brief Main implementation for RoveComm C++
 *
 * @file RoveComm.hpp
 * @author Viktor Butkovich (vsbbkf@umsystem.edu)
 * @date 2023-09-28
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#include "RoveComm.h"
#include "Consts.h"
#include "RoveCommEthernetTcp.h"
#include "RoveCommEthernetUdp.h"
#include "RoveCommPacket.h"

RoveComm::RoveComm(int nUdpPort, ip_address stTcpAddr)
{
    // FIXME: Change Callback and DefaultCallback to use some kind of function objects
    // FIXME: Find an equivalent to Python's threading library

    // Map of specific function call backs for data ids
    Callbacks = {};

    // An optional callback for all incoming packets (can be used for logging, etc)
    // DefaultCallback = NULL;
    // self.default_callback = None

    UdpNode = RoveCommEthernetUdp(nUdpPort);
    TcpNode = RoveCommEthernetTcp(stTcpAddr.szIp, stTcpAddr.nPort);

    // ShutdownEvent = threading.event();
    // ^ Find a C++ equivalent to a thread object
    // Thread = threading.Thread(target=Listen);
    // Thread.start();
    return;
}

void RoveComm::Listen()
{
    TcpNode.HandleIncomingConnection();
    RoveCommPacket* packets = TcpNode.Read();
    for (int index = 0; index < 10; index++)
    {    // use packet length as condition
        RoveCommPacket* packet = &packets[index];
        if (packet != NULL)
        {
            std::invoke(*Callbacks[(*packet).m_nDataId], packet);
            if (DefaultCallback != NULL)
            {
                std::invoke(*DefaultCallback, packet);
            }
        }
    }

    /*
    while(threading.main_thread().is_alive() and !(ShutdownEvent.isSet())){
    TcpNode.HandleIncomingConnection();
        RoveCommPacket* packets = TcpNode.Read();

        packets.append(UdpNode.Read());
        for(int index = 0; index < len(packets); index++){
            RoveCommPacket* packet = packets[index];
            if(packet != NULL){
                try:
                    // Call function in Callbacks[packet.data_id](packet)
                except:
                    pass;
                if(DefaultCallback != NULL){
                    // Call DefaultCallback(packet)
                }
            }
        }
    }
    */
    UdpNode.CloseSocket();
    TcpNode.CloseSockets();

    // FIXME: Find an equivalent to Python version's logging
    // Logger throws an error when logging to console with main thread closed
    // if(threading.main_thread().is_alive())
    //  logging.getLogger(__name)).debug("Rovecomm sockets closed");

    return;
}

void RoveComm::SetCallback(int nDataId, std::function<void(RoveCommPacket*)>* Func)
{
    Callbacks[nDataId] = Func;
    return;
}

void RoveComm::ClearCallback(int nDataId)
{
    Callbacks.erase(nDataId);
    return;
}

void RoveComm::SetDefaultCallback(std::function<void(RoveCommPacket*)>* Func)
{
    DefaultCallback = Func;
    return;
}

void RoveComm::ClearDefaultCallback()
{
    DefaultCallback = NULL;
    return;
}

int RoveComm::Write(RoveCommPacket& Packet, bool bReliable /*= false*/)
{
    if (bReliable)
    {
        return TcpNode.Write(Packet);
    }
    else
    {
        return UdpNode.Write(Packet);
    }
}

void RoveComm::CloseThread()
{
    return;
}
