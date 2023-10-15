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
    return;
}

void RoveComm::SetCallback(int nDataId, std::string& Func)    // Find a way to pass function as argument
{
    return;
}

void RoveComm::ClearCallback(int nDataId)
{
    return;
}

void RoveComm::SetDefaultCallback(std::string& Func)    // Find a way to pass function as argument
{
    return;
}

void RoveComm::ClearDefaultCallback()
{
    return;
}

int RoveComm::Write(RoveCommPacket& Packet, bool bReliable = false)
{
    return 1;
}

void RoveComm::CloseThread()
{
    return;
}

std::map<std::string, std::string> get_manifest(std::string szPath = "")
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
    std::map<std::string, std::string> manifest;

    return (manifest);
}
