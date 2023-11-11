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
#include <sys/socket.h>

RoveCommEthernetUdp::RoveCommEthernetUdp(int nPort /*= ROVECOMM_UDP_PORT*/)
{
    m_nNumSubscribers = 0;
    m_nRoveCommPort   = nPort;

    // socket() returns file descriptor and takes domain, type, and protocol ints, same as a Python socket
    // While a socket object does not exist, SocketFd can be used to refer to the created socket in socket functions
    m_RoveCommSocketFd        = socket(AF_INET, SOCK_DGRAM, -1);

    sockaddr default_sockaddr = {m_nRoveCommPort};

    // C++ sys/socket does not have equivalent to Python's socket.setblocking(True) function

    bind(m_RoveCommSocketFd, &default_sockaddr, 4);
    return;
}

int RoveCommEthernetUdp::Subscribe(std::string szSubToIp)
{
    RoveCommPacket Packet = RoveCommPacket(3, 'b', szSubToIp);
    return Write(Packet);
}

int RoveCommEthernetUdp::Write(const RoveCommPacket& Packet)
{
    try
    {
        std::string szRoveCommPacket;
        szRoveCommPacket += ROVECOMM_HEADER_FORMAT;
        szRoveCommPacket += static_cast<char>(ROVECOMM_VERSION);
        szRoveCommPacket += static_cast<char>(Packet.m_nDataId);
        szRoveCommPacket += static_cast<char>(Packet.m_nDataCount);
        szRoveCommPacket += Packet.m_cDataType;

        for (int i = 0; i < Packet.m_nDataCount; i++)
        {
            szRoveCommPacket += '>' + Packet.m_cDataType + static_cast<char>(i);
        }
        //for (const auto& nCurrentPair : m_aSubscribers)
        for(int i = 0; i < m_nNumSubscribers; i++)
        {
            sendto(m_RoveCommSocketFd, &szRoveCommPacket, sizeof(szRoveCommPacket), 0, m_aSubscribers[i], 4);
        }

        if (!(Packet.m_stIp.nPort == 0 && Packet.m_stIp.stIp.sa_data == "0.0.0.0"))
        {
            sendto(m_RoveCommSocketFd, &szRoveCommPacket, sizeof(szRoveCommPacket), 0, &Packet.m_stIp.stIp, 4);
            // Establish a new connection if the destination has not yet been connected to yet
        }
        return 1;
    }
    catch (...)
    {
        return 0;
    }
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
