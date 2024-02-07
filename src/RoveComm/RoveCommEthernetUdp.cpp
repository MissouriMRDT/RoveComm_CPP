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
#include "LogHack.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

bool RoveCommEthernetUdp::Init()
{
    sockaddr_in sAddress = {
        .sin_family = AF_INET,            // IPv4
        .sin_port   = htons(m_usPort),    // port
        .sin_addr   = {INADDR_ANY}        // use local IP
        // char sin_zero[8] is initialized to 0 for us (padding bytes)
    };

    // open socket
    int m_nSocket = socket(PF_INET,       // AF_INET, but with a PF for historical reasons
                           SOCK_DGRAM,    // UDP
                           0              // choose protocol automatically
    );
    if (m_nSocket == -1)
    {
        LOG_ERROR(logging::g_qSharedLogger, "Failed to create UDP socket!");
        return false;
    }
    // bind the socket to the host ip and port
    if (bind(m_nSocket, (sockaddr*) &sAddress, sizeof(sAddress)) == -1)
    {
        LOG_ERROR(logging::g_qSharedLogger, "Failed to bind UDP socket!");
        close(m_nSocket);
        return false;
    }

    // not sure what nOptVal actually does?
    int nOptVal = 1;
    // Allows the socket address to be reused after being closed
    if (setsockopt(m_nSocket, SOL_SOCKET, SO_REUSEADDR, &nOptVal, sizeof(nOptVal)) == -1)
    {
        LOG_WARNING(logging::g_qSharedLogger, "Warning: setsockopt(SO_REUSEADDR) failed.");
    }
    // Fixes an error on linux with opening the socket again too soon
    if (setsockopt(m_nSocket, SOL_SOCKET, SO_REUSEPORT, &nOptVal, sizeof(nOptVal)) == -1)
    {
        LOG_WARNING(logging::g_qSharedLogger, "Warning: setsockopt(SO_REUSEPORT) failed.");
    }

    // Set up fd_set for reading
    FD_ZERO(&m_sReadSet);
    FD_SET(m_nSocket, &m_sReadSet);

    LOG_INFO(logging::g_qSharedLogger, "Opened UDP server on port {}", m_usPort);
    return true;
}

void RoveCommEthernetUdp::Shutdown()
{
    shutdown(m_nSocket, 1);
    FD_ZERO(&m_sReadSet);
    m_lSubscribers.clear();

    LOG_INFO(logging::g_qSharedLogger, "Closed UDP socket on port {}", m_usPort);
}

int RoveCommEthernetUdp::Write(const RoveCommPacket& packet)
{
    auto [pData, siSize] = RoveCommPacket::Pack(packet);
    int nSuccessful      = 0;
    for (const auto& sAddress : m_lSubscribers)
    {
        int nSentBytes = sendto(m_nSocket, pData.get(), siSize, 0, (sockaddr*) &sAddress, sizeof(sAddress));
        if (nSentBytes <= 0)
        {
            LOG_ERROR(logging::g_qSharedLogger, "A message failed to send!");
            continue;
        }
        else if (nSentBytes < packet.CalcSize())
        {
            LOG_ERROR(logging::g_qSharedLogger, "A message only sent partially!");
            continue;
        }
        nSuccessful++;
    }
    return nSuccessful;
}

int RoveCommEthernetUdp::SendTo(const RoveCommPacket& packet, const RoveCommAddress& address)
{
    auto [pData, siSize] = RoveCommPacket::Pack(packet);

    // pack the octets in address into a 32 bit integer
    long lPackIp;
    char* pPackIp  = reinterpret_cast<char*>(&lPackIp);
    RoveCommIp sIp = address.GetIp();
    pPackIp[0]     = sIp.firstOctet;
    pPackIp[1]     = sIp.secondOctet;
    pPackIp[2]     = sIp.thirdOctet;
    pPackIp[3]     = sIp.fourthOctet;
    // info for sendto
    sockaddr_in sAddress = {
        .sin_family = AF_INET,                 // IPv4
        .sin_port   = htons(m_usPort),         // port
        .sin_addr   = {(in_addr_t) lPackIp}    // use local IP
        // char sin_zero[8] is initialized to 0 for us (padding bytes)
    };

    int nSentBytes = sendto(m_nSocket, pData.get(), siSize, 0, (sockaddr*) &sAddress, sizeof(sAddress));
    if (nSentBytes <= 0)
    {
        LOG_ERROR(logging::g_qSharedLogger, "A message failed to send!");
    }
    else if (nSentBytes < packet.CalcSize())
    {
        LOG_ERROR(logging::g_qSharedLogger, "A message only sent partially!");
    }
    return nSentBytes;
}

// Returns vector of length 0 or 1 which is kind of weird but anything for polymorphism :)
std::vector<RoveCommPacket> RoveCommEthernetUdp::Read()
{
    std::vector<RoveCommPacket> packets;
    fd_set sReadSetCopy = m_sReadSet;
    int nReady          = select(m_nSocket, &sReadSetCopy, NULL, NULL, 0);
    if (nReady == -1)
    {
        LOG_ERROR(logging::g_qSharedLogger, "something went wrong idk");
        return packets;
    }
    if (!FD_ISSET(m_nSocket, &sReadSetCopy))
        return packets;
    // no fancy buffering here because UDP comes in discrete packets, not streams.
    // UDP is also physically constrained in how many bytes it can send in one go.
    // stack allocation is quick so this is fine hopefully
    char pBuf[rovecomm::ROVECOMM_PACKET_MAX_BYTES];
    sockaddr_in sFrom;
    socklen_t sFromLen;
    // reads only one packet at a time, unline TCP recv()
    int nReceived = recvfrom(m_nSocket, pBuf, sizeof(pBuf), 0, (sockaddr*) &sFrom, &sFromLen);
    if (nReceived <= 0)
    {
        LOG_ERROR(logging::g_qSharedLogger, "Failed to receive data!");
        return packets;
    }
    RoveCommPacketHeader sHeader;
    try
    {
        RoveCommPacket::ReadHeader(sHeader, pBuf);

        int nDataLength = sHeader.usDataCount * rovecomm::DataTypeSize(sHeader.ucDataType);
        if (nReceived < nDataLength + rovecomm::ROVECOMM_PACKET_HEADER_SIZE)
            throw std::runtime_error("Incomplete UDP packet received.");

        char* pData = new char[nDataLength];
        RoveCommPacket::ReadData(pData, pBuf, sHeader);
        packets.emplace_back(sHeader, std::unique_ptr<char>(pData));
    }
    catch (std::runtime_error& eErr)
    {
        LOG_ERROR(logging::g_qSharedLogger, "Error: {}", eErr.what());
        return packets;
    }
    if (sHeader.usDataId == rovecomm::System::SUBSCRIBE_DATA_ID)
    {
        m_lSubscribers.push_back(sFrom);
        LOG_INFO(logging::g_qSharedLogger, "Received subscription from {}.", inet_ntoa(sFrom.sin_addr));
    }
    if (sHeader.usDataId == rovecomm::System::UNSUBSCRIBE_DATA_ID)
    {
        m_lSubscribers.remove_if(
            [&sFrom](sockaddr_in el)
            {
                return el.sin_addr.s_addr == sFrom.sin_addr.s_addr &&    // s_addr is a uint_32
                       el.sin_port == sFrom.sin_port;                    // sin_port is a uint_16
            });
        LOG_INFO(logging::g_qSharedLogger, "Received subscription from {}.", inet_ntoa(sFrom.sin_addr));
    }

    return packets;
}

void RoveCommEthernetUdp::Subscribe(const RoveCommAddress& address)
{
    SendTo(RoveCommPacket(rovecomm::System::SUBSCRIBE_DATA_ID), address);
}

void RoveCommEthernetUdp::Unsubscribe(const RoveCommAddress& address)
{
    SendTo(RoveCommPacket(rovecomm::System::UNSUBSCRIBE_DATA_ID), address);
}
