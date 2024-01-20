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
#include "LogHack.h"

#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

void RoveCommEthernetTcp::Init()
{
    struct addrinfo hints, *result;
    std::memset(&hints, 0, sizeof(hints));    // don't use {0} because it does not set padding bytes
    hints.ai_family   = AF_INET;              // IPv4
    hints.ai_socktype = SOCK_STREAM;          // use TCP
    hints.ai_protocol = IPPROTO_TCP;          // idk i found this somewhere
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
        return;
    }

    addrinfo* p = result;
    for (p; p != NULL && p->ai_family == AF_INET; p = p->ai_next)    // getaddrinfo() returns a linked list of possible addresses
    {
        if (m_nListeningSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol) == -1)
        {
            continue;
        }
        // bind the socket to the host ip and port
        if (bind(m_nListeningSocket, result->ai_addr, result->ai_addrlen) == -1)
        {
            close(m_nListeningSocket);
            continue;
        }
        // Accept a backlog of 5 connections, before we start discarding them
        // Why 5? No idea but that's what the Python limit is so best leave it be.
        // According to an ancient webpage, most systems cap this number at 20, so 10 should be safe, but I digress.
        if (listen(m_nListeningSocket, rovecomm::ROVECOMM_ETHERNET_TCP_MAX_CONNECTIONS) == -1)
        {
            close(m_nListeningSocket);
            continue;
        }
        break;
        // bind/listen failed for all sockets
    }
    freeaddrinfo(result);    // *result was allocated by getaddrinfo()
    if (p == NULL)
    {
        LOG_ERROR(logging::g_qSharedLogger, "Failed to open TCP socket!");
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

    // Set up fd_set for reading
    FD_ZERO(&m_sReadSet);
    // Set up fd_set for accepting
    FD_ZERO(&m_sAcceptSet);
    FD_SET(m_nListeningSocket, &m_sAcceptSet);

    LOG_INFO(logging::g_qSharedLogger, "Opened TCP socket on port {}", m_unPort);
}

void RoveCommEthernetTcp::Shutdown()
{
    for (const auto& [sCurrentAddress, nCurrentFd] : m_mOpenSockets)
    {
        // Notifies other end that we are terminating the connection
        shutdown(nCurrentFd, 1);                // Value of 1 stops send() but lets the program finish up remaining recv()'s
        _unregister_socket(sCurrentAddress);    // this could be a problem
    }
    shutdown(m_nListeningSocket, 1);
    FD_ZERO(&m_sReadSet);
    FD_ZERO(&m_sAcceptSet);

    LOG_INFO(logging::g_qSharedLogger, "Closed TCP socket on port {}", m_unPort);
}

int RoveCommEthernetTcp::Write(const RoveCommPacket& packet)
{
    auto [pData, siSize] = RoveCommPacket::Pack(packet);
    //
    int nSuccessful = 0;
    for (const auto& [sAddress, nSocket] : m_mIncomingSockets)
    {
        int nSentBytes = send(nSocket, pData.get(), siSize, 0);    // TODO: write helper function
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
    if (nSentBytes <= 0)
    {
        LOG_ERROR(logging::g_qSharedLogger, "A message failed to send!");
    }
    return nSentBytes;
}

std::vector<RoveCommPacket> RoveCommEthernetTcp::Read()
{
    std::vector<RoveCommPacket> packets;
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
            if (nReceived = recv(nSocket, (void*) (&vBuffer.back() + 1), nHeaderSize, 0) <= 0)
            {
                if (nReceived == 0)
                {
                    close(nSocket);
                    _unregister_socket(sAddress);
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
            try
            {
                RoveCommPacket::ReadHeader(sHeader, &vBuffer[0]);
            }
            catch (std::runtime_error& eErr)
            {
                close(nSocket);
                _unregister_socket(sAddress);
                LOG_ERROR(logging::g_qSharedLogger,
                          "Error at {}: {}\n"
                          "Terminating connection.",
                          sAddress.ToString(),
                          eErr.what());
                continue;
            }
            size_t siTypeSize   = rovecomm::DataTypeSize(sHeader.ucDataType);
            size_t siPacketSize = nHeaderSize + sHeader.usDataCount * siTypeSize;
            int nExpectedBytes  = siPacketSize - vBuffer.size();
            int nPreviousSize   = vBuffer.size();
            vBuffer.resize(nHeaderSize + nExpectedBytes);
            int nReceived = 0;
            if (nReceived = recv(nSocket, (void*) (&vBuffer.back() + 1), nExpectedBytes, 0) <= 0)
            {
                if (nReceived == 0)
                {
                    close(nSocket);
                    _unregister_socket(sAddress);
                    LOG_ERROR(logging::g_qSharedLogger, "Device at address {} disconnected from host", sAddress.ToString());
                }
                else
                {
                    LOG_ERROR(logging::g_qSharedLogger, "Failed to receive data from {}!", sAddress.ToString());
                }
                vBuffer.resize(nPreviousSize);
                continue;
            }
            vBuffer.resize(nPreviousSize + nReceived);
            if (vBuffer.size() >= siPacketSize)    // SUCCESS! Convert to RoveCommPacket!
            {
                char* pData = new char[sHeader.usDataCount * siTypeSize];
                RoveCommPacket::ReadData(pData, &vBuffer[0], sHeader);    // this could throw an error
                vBuffer.erase(vBuffer.begin(), vBuffer.begin() + siPacketSize);
                //
                packets.emplace_back(sHeader, std::unique_ptr<char>(pData));    // construct a new RoveCommPacket
                //
            }    // else wait until next Read() call to read rest of packet
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
    addrinfo* p = result;
    for (p; p != NULL && p->ai_family == AF_INET; p = p->ai_next)
    {
        if (nTcpSocketFd = socket(result->ai_family, result->ai_socktype, result->ai_protocol) == -1)
        {
            continue;
        }
        if (connect(nTcpSocketFd, result->ai_addr, result->ai_addrlen) == -1)
        {
            close(nTcpSocketFd);
            continue;
        }
        break;
    }
    freeaddrinfo(result);    // allocated by getaddrinfo()
    if (p == NULL)
    {
        LOG_ERROR(logging::g_qSharedLogger, "Failed to connect to address {}!", address.ToString());
        return false;
    }
    _register_socket(address, nTcpSocketFd, false);
    LOG_INFO(logging::g_qSharedLogger, "Successfully connected to: {}.", address.ToString());
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
    RoveCommPort unIncomingPort = ntohs(sIncomingAddress.sin_port);                              // convert to host byte order
    char* nReadIp               = reinterpret_cast<char*>(&sIncomingAddress.sin_addr.s_addr);    // network byte order (1.2.3.4)
    RoveCommIp sIncomingIp{nReadIp[0], nReadIp[1], nReadIp[2], nReadIp[3]};
    RoveCommAddress newRoveCommAddress(sIncomingIp, unIncomingPort);

    if (m_mOpenSockets.contains(newRoveCommAddress))    // this shouldn't happen but better safe than sorry
    {
        LOG_ERROR(logging::g_qSharedLogger, "Another device tried to connect, but already had an open TCP connection!");
        return;
    }
    _register_socket(newRoveCommAddress, nIncomingConnection, true);

    LOG_INFO(logging::g_qSharedLogger, "Successfully accepted connection from: {}.", newRoveCommAddress.ToString());
}

void RoveCommEthernetTcp::_register_socket(const RoveCommAddress& sAddress, RoveCommSocket nSocket, bool bIsIncoming)
{
    m_mOpenSockets[sAddress] = nSocket;
    if (bIsIncoming)
        m_mIncomingSockets[sAddress] = nSocket;
    m_mReadBuffers[nSocket] = std::vector<char>();
    FD_SET(nSocket, &m_sReadSet);
    if (nSocket > m_nMaxSocket)
        m_nMaxSocket = nSocket;
}

void RoveCommEthernetTcp::_unregister_socket(const RoveCommAddress& sAddress)
{
    RoveCommSocket nSocket = m_mOpenSockets.at(sAddress);
    m_mOpenSockets.erase(sAddress);
    m_mIncomingSockets.erase(sAddress);    // if it exists
    m_mReadBuffers.erase(nSocket);
    FD_CLR(nSocket, &m_sReadSet);
}
