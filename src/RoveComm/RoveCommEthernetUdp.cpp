/******************************************************************************
 * @brief RoveCommEthernetUdp Implementation for RoveComm C++ Library
 *
 * @file RoveCommEthernetUdp.cpp
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-10-08
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#include "RoveCommEthernetUdp.h"
#include "RoveCommPacket.h"

RoveCommEthernetUdp::RoveCommEthernetUdp(int nPort /*= ROVECOMM_UDP_PORT*/)
{
    return;
}

int RoveCommEthernetUdp::Subscribe(std::string szSubToIp)
{
    return 1;
}

int RoveCommEthernetUdp::Write(RoveCommPacket& Packet)
{
    return 1;
}

RoveCommPacket* RoveCommEthernetUdp::Read()
{
    RoveCommPacket* rcReturn = new RoveCommPacket();
    return rcReturn;
}

void RoveCommEthernetUdp::CloseSocket()
{
    return;
}
