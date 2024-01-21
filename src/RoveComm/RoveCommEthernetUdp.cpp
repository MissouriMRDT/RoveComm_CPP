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
    struct addrinfo hints, *result;
    std::memset(&hints, 0, sizeof(hints));    // don't use {0} because it does not set padding bytes
    hints.ai_family   = AF_INET;              // IPv4
    hints.ai_socktype = SOCK_DGRAM;           // use UDP
    hints.ai_protocol = IPPROTO_UDP;          // idk i found this somewhere
    hints.ai_flags    = AI_PASSIVE;           // fill in  host IP automatically

    // super magic function that lets us avoid packing a sockaddr_in struct manually
    // and lets us avoid calling gethostbyname(gethostname())
    // it also makes it easier to add IPv6 if we ever do which is unlikely
    // format: getaddrinfo(char* ip, char* port, addrinfo* settings, addrinfo** linked list of results)
    // if ip is NULL and AI_PASSIVE flag is set, then the host's ip will be used
    // result will actually be a linked list but for now we just get the first entry
    if (int status = getaddrinfo(NULL, std::to_string(m_unPort).c_str(), &hints, &result) != 0)
    {
        LOG_ERROR(logging::g_qSharedLogger, "Failed to find IP! Error: {}", gai_strerror(status));
        return false;
    }

    addrinfo* p = result;
    for (p; p != NULL && p->ai_family == AF_INET; p = p->ai_next)    // getaddrinfo() returns a linked list of possible addresses
    {
        if (m_nSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol) == -1)
        {
            continue;
        }
        // bind the socket to the host ip and port
        if (bind(m_nSocket, result->ai_addr, result->ai_addrlen) == -1)
        {
            close(m_nSocket);
            continue;
        }
        break;
    }
    freeaddrinfo(result);    // *result was allocated by getaddrinfo()
    if (p == NULL)
    {
        LOG_ERROR(logging::g_qSharedLogger, "Failed to open UDP socket!");
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

    LOG_INFO(logging::g_qSharedLogger, "Opened UDP server on port {}", m_unPort);
    return true;
}

void RoveCommEthernetUdp::Shutdown()
{
    shutdown(m_nSocket, 1);
    FD_ZERO(&m_sReadSet);
    m_lSubscribers.clear();

    LOG_INFO(logging::g_qSharedLogger, "Closed UDP socket on port {}", m_unPort);
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
        nSuccessful++;
    }
    return nSuccessful;
}

int RoveCommEthernetUdp::SendTo(const RoveCommPacket& packet, RoveCommAddress address)
{
    auto [pData, siSize] = RoveCommPacket::Pack(packet);

    struct addrinfo hints, *result;
    std::memset(&hints, 0, sizeof(hints));    // don't use {0} because it does not set padding bytes
    hints.ai_family   = AF_INET;              // IPv4
    hints.ai_socktype = SOCK_DGRAM;           // use UDP
                                              // do not specify AI_PASSIVE when using sendto()
    hints.ai_protocol = IPPROTO_UDP;          // idk i found this somewhere

    if (int status = getaddrinfo(address.GetIp().ToString().c_str(), std::to_string(m_unPort).c_str(), &hints, &result) != 0)
    {
        LOG_ERROR(logging::g_qSharedLogger, "Failed to find IP! Error: {}", gai_strerror(status));
        return 0;
    }
    // I'll just be lazy so I won't do the for loop shenanigans and use the first value
    int nSentBytes = sendto(m_nSocket, pData.get(), siSize, 0, result->ai_addr, result->ai_addrlen);
    freeaddrinfo(result);    // *result was allocated by getaddrinfo()
    if (nSentBytes <= 0)
    {
        LOG_ERROR(logging::g_qSharedLogger, "A message failed to send!");
    }
    return nSentBytes;
}

// Returns vector of length 0 or 1 which is kind of weird but anything for polymorphism :)
std::vector<RoveCommPacket> RoveCommEthernetUdp::Read()
{
    std::vector<RoveCommPacket> packets;
    fd_set sReadSetCopy = m_sReadSet;
    int nReady;
    if (nReady = select(m_nSocket, &sReadSetCopy, NULL, NULL, 0) == -1)
    {
        LOG_ERROR(logging::g_qSharedLogger, "something went wrong idk");
        return packets;
    }
    if (!FD_ISSET(m_nSocket, &sReadSetCopy))
        return packets;
    // no fancy buffering here because UDP comes in discrete packets, not streams.
    // UDP is also physically constrained in how many bytes it can send in one go.
    // stack allocation is quick so this is fine hopefully
    char pBuf[rovecomm::ROVECOMM_PACKET_MAX_DATA_COUNT];
    sockaddr_in sFrom;
    socklen_t sFromLen;
    int nReceived;
    if (nReceived = recvfrom(m_nSocket, pBuf, sizeof(pBuf), 0, (sockaddr*) &sFrom, &sFromLen) <= 0)
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
    SendTo(RoveCommPacket(rovecomm::System::SUBSCRIBE_DATA_ID), address);
}
