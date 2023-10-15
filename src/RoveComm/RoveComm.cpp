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
#include "string"
#include <map>

RoveComm::RoveComm(int nUdpPort, int nTcpAddr)
{
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
