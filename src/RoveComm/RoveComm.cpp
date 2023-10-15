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
#include "string"
#include <map>

RoveComm::RoveComm(int nUdpPort, ip_address& stTcpAddr)
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

void RoveComm::SetCallback(int nDataId, std::string& Func)    // Find a way to pass function as argument
{
    Callbacks[nDataId] = Func;
    return;
}

void RoveComm::ClearCallback(int nDataId)
{
    Callbacks.erase(nDataId);
    return;
}

void RoveComm::SetDefaultCallback(std::string& Func)    // Find a way to pass function as argument
{
    DefaultCallback = Func;
    return;
}

void RoveComm::ClearDefaultCallback()
{
    // FIXME: Maybe implement DefaultCallback to a pointer to a function, so that it can be cleared by pointing to NULL
    // DefaultCallback = NULL;
    return;
}

int RoveComm::Write(RoveCommPacket& Packet, bool bReliable = false)
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

std::map<std::string, std::string> GetManifest(std::string szPath = "")
{
    /*
    Grabs the json manifest file and returns it in map form

    Parameters:
    -----------
        path - the path to a specified manifest file. If left blank we default
        to manifest found in this repo
    Returns:
    --------
        manifest - the manifest in dictionary form
    */

    // FIXME: Find way to load a JSON file and convert it to a map, and return this map

    std::map<std::string, std::string> manifest;

    return (manifest);
}
