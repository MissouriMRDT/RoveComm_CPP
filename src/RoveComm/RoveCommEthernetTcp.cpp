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

#include <chrono>
#include <cstring>
#include <iostream>
#include <memory>
#include <mutex>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

void RoveCommEthernetTcp::Init()
{
    struct addrinfo hints, *result;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;    // use TCP
    hints.ai_flags    = AI_PASSIVE;     // fill in  host IP automatically

    // super magic function that lets us avoid packing a sockaddr_in struct manually
    // and lets us avoid calling gethostbyname(gethostname())
    // it also makes it easier to add IPv6 if we ever do which is unlikely
    // format: getaddrinfo(char* ip, char* port, addrinfo* settings, addrinfo** linked list of results)
    // if ip is NULL and AI_PASSIVE flag is set, then the host's ip will be used
    // result will actually be a linked list but for now we just get the first entry
    if (int status = getaddrinfo(NULL, std::to_string(m_unPort).c_str(), &hints, &result) != 0)
    {
        // error
        std::cout << "Failed to find IP! : " << gai_strerror(status) << std::endl;
        exit(1);
        return;
    }

    // socket() returns an integer file descriptor and takes domain, type, and protocol ints, same as a Python socket
    if (m_nListeningSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol) == -1)
    {
        // error
        std::cout << "Failed to create socket!";
        return;
    }

    // not sure what nOptVal actually does?

    int nOptVal = 1;
    // Allows the socket address to be reused after being closed
    if (setsockopt(m_nListeningSocket, SOL_SOCKET, SO_REUSEADDR, &nOptVal, sizeof(nOptVal)) == -1)
    {
        std::cout << "Warning: setsockopt(SO_REUSEADDR) failed.";
    }
    // Fixes an error on linux with opening the socket again too soon
    if (setsockopt(m_nListeningSocket, SOL_SOCKET, SO_REUSEPORT, &nOptVal, sizeof(nOptVal)) == -1)
    {
        std::cout << "Warning: setsockopt(SO_REUSEPORT) failed.";
    }

    // bind the socket to the host ip and port
    if (bind(m_nListeningSocket, result->ai_addr, result->ai_addrlen) == -1)
    {
        close(m_nListeningSocket);
        std::cout << "Failed to bind socket!";
        return;
    }

    // Accept a backlog of 5 connections, before we start discarding them
    // Why 5? No idea but that's what the Python limit is so best leave it be.
    // According to an ancient webpage, most systems cap this number at 20, so 10 should be safe, but I digress.
    if (listen(m_nListeningSocket, rovecomm::MAX_SOCKET_QUEUE_LENGTH) == -1)
    {
        std::cout << "Damned socket won't listen >:(";
        close(m_nListeningSocket);
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
    for (const auto& nCurrentPair : m_mOpenSockets)
    {
        RoveCommSocket nCurrentFd = nCurrentPair.second;

        // Notifies other end that we are terminating the connection
        shutdown(nCurrentFd, 1);    // Value of 1 stops send() but lets the program finish up remaining recv()'s
        // close(nCurrentFd);
    }
    shutdown(m_nListeningSocket, 1);
    // close(m_nListeningSocket);

    return;
}

int RoveCommEthernetTcp::Write(RoveCommPacket& packet) const
{
    RoveCommByteBuffer buffer = RoveCommPacket::Pack(packet);

    int nSuccessful           = 0;
    for (const auto& nCurrentPair : m_mOpenSockets)
    {
        int nCurrentFd = nCurrentPair.second;
        int sent       = send(nCurrentFd, buffer.GetRawData(), buffer.GetSize(), 0);
        if (sent <= 0)
        {
            std::cout << "A message failed to send!" << std::endl;
            continue;
        }
        nSuccessful++;
    }
    return nSuccessful;
}

int RoveCommEthernetTcp::SendTo(RoveCommPacket& packet, RoveCommAddress address)
{
    // Establish a new connection if the destination has not yet been connected to yet
    if (!Connect(address))
    {
        return -1;
    }
    RoveCommByteBuffer buffer = RoveCommPacket::Pack(packet);
    int nBytesSent            = send(m_mOpenSockets.at(address), buffer.GetRawData(), buffer.GetSize(), 0);
    return nBytesSent;
}

std::vector<std::unique_ptr<RoveCommPacket>> RoveCommEthernetTcp::Read() const    // needs to return pointer to array of RoveCommPackets
{
    auto packets = std::vector<std::unique_ptr<RoveCommPacket>>();
    // determine which sockets have data waiting
    fd_set sReadSetCopy = m_sReadSet;
    int nReady;
    if (nReady = select(m_nMaxSocket, &sReadSetCopy, NULL, NULL, 0) == -1)
    {
        std::cout << "something went wrong idk";
        return;
    }
    if (nReady == 0)    // no packets to read
        return;

    for (const auto& pair : m_mOpenSockets)
    {
        RoveCommSocket nOpenSocket = pair.second;
        if (!FD_ISSET(nOpenSocket, &sReadSetCopy))
            continue;
        // you got mail!
        // read into buffer and stuff and add to packets[]
        // make sure to FD_CLR() if recv returns 0!
    }
    return packets;
}

bool RoveCommEthernetTcp::Connect(RoveCommAddress& address)
{
    if (m_mOpenSockets.contains(address))
    {
        std::cout << "Host tried to connect to address " << address << ", but already had an open connection." << std::endl;
        return false;
    }
    struct addrinfo hints, *result;
    RoveCommSocket nTcpSocketFd;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(address.GetIp().ToString().c_str(), std::to_string(m_unPort).c_str(), &hints, &result);

    if (nTcpSocketFd = socket(result->ai_family, result->ai_socktype, result->ai_protocol) == -1)
    {
        std::cout << "Failed to create socket!" << std::endl;
        return false;
    }
    if (connect(nTcpSocketFd, result->ai_addr, result->ai_addrlen) == -1)
    {
        std::cout << "Failed to connect to address: " << address << "!" << std::endl;
        close(nTcpSocketFd);
        return false;
    }
    m_mOpenSockets.emplace(address, nTcpSocketFd);
    FD_SET(nTcpSocketFd, &m_sReadSet);
    if (nTcpSocketFd > m_nMaxSocket)
        m_nMaxSocket = nTcpSocketFd;
    std::cout << "Successfully connected to: " << address << std::endl;
    return true;
}

void RoveCommEthernetTcp::AcceptIncomingConnections()
{
    sockaddr_in sIncomingAddress;
    socklen_t sAddressSize = sizeof(sIncomingAddress);

    fd_set sAcceptSetCopy  = m_sAcceptSet;
    if (select(m_nListeningSocket, &sAcceptSetCopy, NULL, NULL, 0) == -1)
    {
        std::cout << "something went wrong idk";    // exit(1);
        return;
    }
    if (!FD_ISSET(m_nListeningSocket, &sAcceptSetCopy))
        return;
    // accept a connection request from another device, if one exists
    RoveCommSocket nIncomingConnection;
    if (nIncomingConnection = accept(m_nListeningSocket, (struct sockaddr*) &sIncomingAddress, &sAddressSize) == -1)
    {
        std::cout << "Failed to accept connection!" << std::endl;
        return;
    }
    if (sIncomingAddress.sin_family != AF_INET)
    {
        std::cout << "Another device tried to connect with an unsupported IP protocol! Only IPv4 is supported." << std::endl;
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
        std::cout << "Another device tried to connect, but already had an open TCP connection!" << std::endl;
        return;
    }
    m_mOpenSockets.emplace(newRoveCommAddress, nIncomingConnection);
    FD_SET(nIncomingConnection, &m_sReadSet);
    if (nIncomingConnection > m_nMaxSocket)
        m_nMaxSocket = nIncomingConnection;
    std::cout << "Successfully accepted connection from: " << newRoveCommAddress << std::endl;
}
