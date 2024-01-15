/******************************************************************************
 * @brief RoveCommEthernetTcp Implementation for RoveComm C++ Library
 *
 * @file RoveCommEthernetTcp.cpp
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-10-08
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#include "RoveCommEthernetTcp.h"
#include "../RoveCommLogging.h"

#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void RoveCommEthernetTcp::Init()
{
    struct addrinfo hints, *result;
    std::memset(&hints, 0, sizeof(hints));    // don't use {0} because it does not set padding bytes
    hints.ai_family   = AF_INET;              // IPv4
    hints.ai_socktype = SOCK_STREAM;          // use TCP
    hints.ai_flags    = AI_PASSIVE;           // fill in  host IP automatically

    // super magic function that lets us avoid packing a sockaddr_in struct manually
    // and lets us avoid calling gethostbyname(gethostname())
    // it also makes it easier to add IPv6 if we ever do which is unlikely
    // format: getaddrinfo(char* ip, char* port, addrinfo* settings, addrinfo** linked list of results)
    // if ip is NULL and AI_PASSIVE flag is set, then the host's ip will be used
    // result will actually be a linked list but for now we just get the first entry
    if (int status = getaddrinfo(NULL, std::to_string(m_unPort).c_str(), &hints, &result) != 0)
    {
        // error
        LOG_ERROR(logging::g_qSharedLogger, "Failed to find IP! Error: {}", gai_strerror(status));
        // exit(1);
        return;
    }

    // socket() returns an integer file descriptor and takes domain, type, and protocol ints, same as a Python socket
    if (m_nListeningSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol) == -1)
    {
        freeaddrinfo(result);
        LOG_ERROR(logging::g_qSharedLogger, "Failed to create listening socket!");
        return;
    }

    // not sure what nOptVal actually does?

    int nOptVal = 1;
    // Allows the socket address to be reused after being closed
    if (setsockopt(m_nListeningSocket, SOL_SOCKET, SO_REUSEADDR, &nOptVal, sizeof(nOptVal)) == -1)
    {
        LOG_WARNING(logging::g_qSharedLogger, "Warning: setsockopt(SO_REUSEADDR) failed.");
    }
    // Fixes an error on linux with opening the socket again too soon
    if (setsockopt(m_nListeningSocket, SOL_SOCKET, SO_REUSEPORT, &nOptVal, sizeof(nOptVal)) == -1)
    {
        LOG_WARNING(logging::g_qSharedLogger, "Warning: setsockopt(SO_REUSEPORT) failed.");
    }

    // bind the socket to the host ip and port
    if (bind(m_nListeningSocket, result->ai_addr, result->ai_addrlen) == -1)
    {
        freeaddrinfo(result);
        close(m_nListeningSocket);
        LOG_ERROR(logging::g_qSharedLogger, "Failed to bind socket!");
        return;
    }

    // Accept a backlog of 5 connections, before we start discarding them
    // Why 5? No idea but that's what the Python limit is so best leave it be.
    // According to an ancient webpage, most systems cap this number at 20, so 10 should be safe, but I digress.
    if (listen(m_nListeningSocket, rovecomm::MAX_SOCKET_QUEUE_LENGTH) == -1)
    {
        freeaddrinfo(result);
        close(m_nListeningSocket);
        LOG_ERROR(logging::g_qSharedLogger, "Damned socket won't listen >:(");
        return;
    }

    freeaddrinfo(result);    // *result was allocated by getaddrinfo()
    // Set up fd_set for reading
    FD_ZERO(&m_sReadSet);
    // Set up fd_set for accepting
    FD_ZERO(&m_sAcceptSet);
    FD_SET(m_nListeningSocket, &m_sAcceptSet);
}

void RoveCommEthernetTcp::Shutdown()
{
    for (const auto& [sCurrentAddress, nCurrentFd] : m_mOpenSockets)
    {
        // Notifies other end that we are terminating the connection
        shutdown(nCurrentFd, 1);                            // Value of 1 stops send() but lets the program finish up remaining recv()'s
        _unregister_socket(sCurrentAddress, nCurrentFd);    // this could be a problem
    }
    shutdown(m_nListeningSocket, 1);
    FD_ZERO(&m_sReadSet);
    FD_ZERO(&m_sAcceptSet);
    return;
}

int RoveCommEthernetTcp::Write(const RoveCommPacket& packet)
{
    auto [pData, siSize] = RoveCommPacket::Pack(packet);
    //
    int nSuccessful = 0;
    for (const auto& [sAddress, nSocket] : m_mOpenSockets)
    {
        int nSentBytes = send(nSocket, pData.get(), siSize, 0);
        if (nSentBytes <= 0)
        {
            LOG_ERROR(logging::g_qSharedLogger, "A message failed to send!");
            continue;
        }
        nSuccessful++;
    }
    return nSuccessful;
}

int RoveCommEthernetTcp::SendTo(const RoveCommPacket& packet, RoveCommAddress address)
{
    // Establish a new connection if the destination has not yet been connected to yet
    if (!Connect(address))
    {
        return -1;
    }
    auto [pData, siSize] = RoveCommPacket::Pack(packet);
    int nSentBytes       = send(m_mOpenSockets.at(address), pData.get(), siSize, 0);
    return nSentBytes;
}

std::vector<const RoveCommPacket> RoveCommEthernetTcp::Read()
{
    auto packets = std::vector<const RoveCommPacket>();
    // determine which sockets have data waiting
    fd_set sReadSetCopy = m_sReadSet;
    int nReady;
    if (nReady = select(m_nMaxSocket, &sReadSetCopy, NULL, NULL, 0) == -1)
    {
        LOG_ERROR(logging::g_qSharedLogger, "something went wrong idk");
        return packets;
    }
    if (nReady == 0)    // no packets to read
        return packets;

    for (const auto& [sAddress, nSocket] : m_mOpenSockets)
    {
        if (!FD_ISSET(nSocket, &sReadSetCopy))
            continue;
        // you got mail!
        auto& vBuffer = m_mReadBuffers.at(nSocket);

        // attempt to read header if there is nothing in the buffer
        int nHeaderSize = rovecomm::ROVECOMM_PACKET_HEADER_SIZE;
        if (vBuffer.size() < nHeaderSize)
        {
            int nPreviousSize = vBuffer.size();
            vBuffer.resize(nPreviousSize + nHeaderSize);    // this is probably UB but idc
            int nReceived = 0;
            if (nReceived = recv(nSocket, ((void*) &vBuffer.back()) + 1, nHeaderSize, 0) <= 0)
            {
                if (nReceived == 0)
                {
                    close(nSocket);
                    _unregister_socket(sAddress, nSocket);
                    LOG_ERROR(logging::g_qSharedLogger, "Device at address {} disconnected from host.", sAddress.ToString());
                }
                else
                {
                    LOG_ERROR(logging::g_qSharedLogger, "Failed to receive data!");
                }
                vBuffer.resize(nPreviousSize);
                continue;
            }
            vBuffer.resize(nPreviousSize + nReceived);    // resize back down in case enough bytes weren't read
        }
        // otherwise parse the header, then attempt to read the rest of the packet if there are enough bytes
        else
        {
            RoveCommPacketHeader sHeader;
            sHeader.ucVersionId = vBuffer[0];
            if (sHeader.ucVersionId != rovecomm::ROVECOMM_VERSION)
            {
                close(nSocket);
                _unregister_socket(sAddress, nSocket);
                LOG_ERROR(logging::g_qSharedLogger,
                          "Packet of unknown version received from {}; terminating connection.\n"
                          "It is possible that a packet lied about its type or length.",
                          sAddress.ToString());
                continue;
            }
            sHeader.usDataId    = (vBuffer[1] << 8) | vBuffer[2];    // network order
            sHeader.usDataCount = (vBuffer[3] << 8) | vBuffer[4];
            sHeader.ucDataType  = vBuffer[5];

            size_t siTypeSize   = 0;
            if (siTypeSize = rovecomm::DataTypeSize(sHeader.ucDataType) == -1)
            {
                close(nSocket);
                _unregister_socket(sAddress, nSocket);
                LOG_ERROR(logging::g_qSharedLogger,
                          "Packet of unknown data type received from {}; terminating connection.\n"
                          "It is possible that a packet lied about its type or length.",
                          sAddress.ToString());
                continue;
            }
            int nExpectedBytes = sHeader.usDataCount * siTypeSize;
            int nPreviousSize  = vBuffer.size();
            vBuffer.resize(nHeaderSize + nExpectedBytes);
            int nReceived = 0;
            if (nReceived = recv(nSocket, ((void*) &vBuffer.back()) + 1, nExpectedBytes, 0) <= 0)
            {
                if (nReceived == 0)
                {
                    close(nSocket);
                    _unregister_socket(sAddress, nSocket);
                    LOG_ERROR(logging::g_qSharedLogger, "Device at address {} disconnected from host", sAddress.ToString());
                }
                else
                {
                    LOG_ERROR(logging::g_qSharedLogger, "Failed to receive data!");
                }
                vBuffer.resize(nPreviousSize);
                continue;
            }
            vBuffer.resize(nPreviousSize + nReceived);
            if (vBuffer.size() >= nHeaderSize + nExpectedBytes)
            {    // SUCCESS! Convert to RoveCommPacket!
                char* pData = new char[nExpectedBytes];
                std::memcpy(pData, &vBuffer[nHeaderSize], nExpectedBytes);
                vBuffer.erase(vBuffer.begin(), vBuffer.begin() + nHeaderSize + nExpectedBytes);    // Clayton wants to parallelize this
                //
                packets.emplace_back(sHeader, std::unique_ptr<char>(pData));    // construct a new RoveCommPacket
            }
        }
    }
    // for all buffers with enough bytes to read a header, attempt to read body
    return packets;
}

bool RoveCommEthernetTcp::Connect(const RoveCommAddress& address)
{
    if (m_mOpenSockets.contains(address))
    {
        // std::cout << "Host tried to connect to address " << address << ", but already had an open connection." << std::endl;
        return true;
    }
    struct addrinfo hints, *result;
    std::memset(&hints, 0, sizeof(hints));
    RoveCommSocket nTcpSocketFd;
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (int status = getaddrinfo(address.GetIp().ToString().c_str(), std::to_string(m_unPort).c_str(), &hints, &result) != 0)
    {
        LOG_ERROR(logging::g_qSharedLogger, "Failed to find IP! Error: {}", gai_strerror(status));
        return false;
    }

    if (nTcpSocketFd = socket(result->ai_family, result->ai_socktype, result->ai_protocol) == -1)
    {
        freeaddrinfo(result);
        LOG_ERROR(logging::g_qSharedLogger, "Failed to create socket!");
        return false;
    }
    if (connect(nTcpSocketFd, result->ai_addr, result->ai_addrlen) == -1)
    {
        freeaddrinfo(result);
        close(nTcpSocketFd);
        LOG_ERROR(logging::g_qSharedLogger, "Failed to connect to address {}!", address.ToString());
        return false;
    }
    freeaddrinfo(result);    // allocated by getaddrinfo()
    _register_socket(address, nTcpSocketFd);
    if (nTcpSocketFd > m_nMaxSocket)
        m_nMaxSocket = nTcpSocketFd;
    LOG_ERROR(logging::g_qSharedLogger, "Successfully connected to: {}.", address.ToString());
    return true;
}

void RoveCommEthernetTcp::AcceptIncomingConnections()
{
    sockaddr_in sIncomingAddress;
    socklen_t sAddressSize = sizeof(sIncomingAddress);

    fd_set sAcceptSetCopy  = m_sAcceptSet;
    if (select(m_nListeningSocket, &sAcceptSetCopy, NULL, NULL, 0) == -1)
    {
        LOG_ERROR(logging::g_qSharedLogger, "something went wrong idk");    // exit(1);
        return;
    }
    if (!FD_ISSET(m_nListeningSocket, &sAcceptSetCopy))
        return;
    // accept a connection request from another device, if one exists
    RoveCommSocket nIncomingConnection;
    if (nIncomingConnection = accept(m_nListeningSocket, (struct sockaddr*) &sIncomingAddress, &sAddressSize) == -1)
    {
        LOG_ERROR(logging::g_qSharedLogger, "Failed to accept connection!");
        return;
    }
    if (sIncomingAddress.sin_family != AF_INET)
    {
        LOG_ERROR(logging::g_qSharedLogger, "Another device tried to connect with an unsupported IP protocol! Only IPv4 is supported.");
        close(nIncomingConnection);
        return;
    }

    // The following code is IPv4-specific. If you are a future developer switching to IPv6, use sockaddr_storage instead of sockaddr_in

    // Read back the address for storage in m_mIncomingSockets
    RoveCommPort unIncomingPort = ntohs(sIncomingAddress.sin_port);    // convert to host byte order
    long int nReadIp            = sIncomingAddress.sin_addr.s_addr;    // network byte order (1.2.3.4)
    RoveCommIp sIncomingIp{nReadIp >> 3 & 0xFF, nReadIp >> 2 & 0xFF, nReadIp >> 1 & 0xFF, nReadIp >> 0 & 0xFF};
    RoveCommAddress newRoveCommAddress(sIncomingIp, unIncomingPort);

    if (m_mOpenSockets.contains(newRoveCommAddress))    // this shouldn't happen but better safe than sorry
    {
        LOG_ERROR(logging::g_qSharedLogger, "Another device tried to connect, but already had an open TCP connection!");
        return;
    }
    _register_socket(newRoveCommAddress, nIncomingConnection);
    if (nIncomingConnection > m_nMaxSocket)
        m_nMaxSocket = nIncomingConnection;
    LOG_ERROR(logging::g_qSharedLogger, "Successfully accepted connection from: {}.", newRoveCommAddress.ToString());
}

void RoveCommEthernetTcp::_register_socket(const RoveCommAddress& sAddress, RoveCommSocket nSocket)
{
    m_mOpenSockets.emplace(sAddress, nSocket);
    m_mReadBuffers.emplace(nSocket);
    FD_SET(nSocket, &m_sReadSet);
}

void RoveCommEthernetTcp::_unregister_socket(const RoveCommAddress& sAddress, RoveCommSocket nSocket)
{
    m_mOpenSockets.erase(sAddress);
    m_mReadBuffers.erase(nSocket);
    FD_CLR(nSocket, &m_sReadSet);
}
