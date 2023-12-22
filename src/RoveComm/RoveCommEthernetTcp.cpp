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
    bind(m_nListeningSocket, result->ai_addr, result->ai_addrlen);

    // Accept a backlog of 5 connections, before we start discarding them
    // Why 5? No idea but that's what the Python limit is so best leave it be.
    // According to an ancient webpage, most systems cap this number at 20, so 10 should be safe, but I digress.
    listen(m_nListeningSocket, rovecomm::MAX_SOCKET_QUEUE_LENGTH);
}

void RoveCommEthernetTcp::Shutdown()
{
    for (const auto& nCurrentPair : m_mOutgoingSockets)
    {
        RoveCommSocket nCurrentFd = nCurrentPair.second;

        // Notifies other end that we are terminating the connection
        shutdown(nCurrentFd, 1);    // Value of 1 stops send() but lets the program finish up remaining recv()'s
        // close(nCurrentFd);
    }
    for (const auto& nCurrentPair : m_mIncomingSockets)
    {
        RoveCommSocket nCurrentFd = nCurrentPair.second;
        shutdown(nCurrentFd, 1);
        // close(nCurrentFd);
    }
    shutdown(m_nListeningSocket, 1);
    // close(m_nListeningSocket);

    return;
}

int RoveCommEthernetTcp::Write(RoveCommPacket& packet) const
{
    // std::string szRoveCommPacket;
    // szRoveCommPacket += ROVECOMM_HEADER_FORMAT;
    // szRoveCommPacket += static_cast<char>(ROVECOMM_VERSION);
    // szRoveCommPacket += static_cast<char>(Packet.m_nDataId);
    // szRoveCommPacket += static_cast<char>(Packet.m_nDataCount);
    // szRoveCommPacket += Packet.m_cDataType;

    char* blob = new char[packet.size()];

    for (int i = 0; i < Packet.m_nDataCount; i++)
    {
        szRoveCommPacket += '>' + Packet.m_cDataType + static_cast<char>(i);
    }
    for (const auto& nCurrentPair : m_IncomingSockets)
    {
        int nCurrentFd = nCurrentPair.second;

        // Notifies other end that we are terminating the connection
        send(nCurrentFd, &szRoveCommPacket, sizeof(szRoveCommPacket), 0);
    }
    if (!(Packet.m_stIp.nPort == 0 && Packet.m_stIp.stIp.sa_data == "0.0.0.0"))
    {
        if (Connect(Packet.m_stIp.stIp) == 0)
        {
            return 0;
        }
        send(m_mOutgoingSockets[Packet.m_stIp.stIp.sa_data], &szRoveCommPacket, sizeof(szRoveCommPacket), 0);
        // Establish a new connection if the destination has not yet been connected to yet
        return 1;
    }
    delete[] blob;
}

std::vector<RoveCommPacket> RoveCommEthernetTcp::Read()    // needs to return pointer to array of RoveCommPackets
{
    int aAvailableSockets[5];
    int packetLength = 0;
    // FIX ME: Add an actual length to packets array
    RoveCommPacket packets[5];
    int currentIndex         = 0;
    RoveCommPacket* rcReturn = new RoveCommPacket();
    for (const auto& nCurrentPair : m_mOutgoingSockets)
    {
        aAvailableSockets[currentIndex] = nCurrentPair.second;
        currentIndex++;
    }

    if (m_nOpenSocketLength > 0)
    {
        // available_sockets = select.select(available_sockets, [], [], 0)[0]
    }
    return rcReturn;

    for (int i = 0; i < m_nOpenSocketLength; i++)
    {
        int OpenSocket = aAvailableSockets[i];
        try
        {
            int buffer      = m_Buffers[getpeername(OpenSocket, NULL, NULL)];
            int nHeaderSize = sizeof(ROVECOMM_HEADER_FORMAT);
            int nHeader     = recv(OpenSocket, &buffer, nHeaderSize, 0);
            return;
        }
        catch (...)
        {
            RoveCommPacket returnPacket = RoveCommPacket();
            packets[packetLength]       = returnPacket;
            packetLength++;
        }
    }
    return packets;
}

void RoveCommEthernetTcp::Connect(RoveCommAddress& address)
{
    if (m_mOutgoingSockets.contains(address))
    {
        std::cout << "Host tried to connect to address " << address << ", but already had an open connection.";
        return;
    }
    struct addrinfo hints, *result;
    RoveCommSocket nTcpSocketFd;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(address.GetIp().ToString().c_str(), std::to_string(m_unPort).c_str(), &hints, &result);

    if (nTcpSocketFd = socket(result->ai_family, result->ai_socktype, result->ai_protocol) == -1)
    {
        std::cout << "Failed to create socket!";
        return;
    }
    if (connect(nTcpSocketFd, result->ai_addr, result->ai_addrlen) == -1)
    {
        std::cout << "Failed to connect to address " << address << "!";
        return;
    }
    m_mOutgoingSockets.emplace(address, nTcpSocketFd);
    std::cout << "Successfully connected to: " << address << std::endl;
}

void RoveCommEthernetTcp::AcceptIncomingConnections()
{
    sockaddr_in sIncomingAddress;
    socklen_t sAddressSize = sizeof(sIncomingAddress);
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

    if (m_mIncomingSockets.contains(newRoveCommAddress))    // this shouldn't happen but better safe than sorry
    {
        std::cout << "Another device tried to connect, but already had an open TCP connection!" << std::endl;
        return;
    }
    m_mIncomingSockets.emplace(newRoveCommAddress, nIncomingConnection);
    std::cout << "Successfully accepted client request: " << newRoveCommAddress << std::endl;
}

/*

    packets = []

    if len(available_sockets) > 0:
        # The select function is used to poll the socket and check whether
        # there is data available to be read, preventing the read from
        # blocking the thread while waiting for a packet
        available_sockets = select.select(available_sockets, [], [], 0)[0]

    for open_socket in available_sockets:
        try:
            buffer = self.buffers[open_socket.getpeername()]
            header_size = struct.calcsize(ROVECOMM_HEADER_FORMAT)
            header = open_socket.recv(header_size)
            buffer.extend(header)

            # If we have enough bytes for the header, parse those
            if len(self.buffers[open_socket.getpeername()]) >= header_size:
                rovecomm_version, data_id, data_count, data_type = struct.unpack(ROVECOMM_HEADER_FORMAT, header)
                data_type_byte = types_int_to_byte[data_type]
                data = open_socket.recv(data_count * types_byte_to_size[data_type_byte])
                buffer.extend(data)

                # If we have enough bytes for header + expected packet size, parse those
                if len(buffer) >= data_count * types_byte_to_size[data_type_byte] + header_size:
                    if rovecomm_version != ROVECOMM_VERSION:
                        returnPacket = RoveCommPacket(ROVECOMM_INCOMPATIBLE_VERSION, "b", (1,), "")
                        returnPacket.SetIp(*open_socket.getpeername())
                        packets.append(returnPacket)
                        # Remove the parsed packet bytes from buffer
                        buffer = buffer[data_count * types_byte_to_size[data_type_byte] + header_size:]

                    else:
                        data_type = types_int_to_byte[data_type]
                        data = struct.unpack(">" + data_type * data_count, data)

                        returnPacket = RoveCommPacket(data_id, data_type, data, "")
                        returnPacket.SetIp(*open_socket.getpeername())
                        packets.append(returnPacket)
                        # Remove the parsed packet bytes from buffer
                        buffer = buffer[data_count * types_byte_to_size[data_type_byte] + header_size:]

        except Exception:
            returnPacket = RoveCommPacket()
            packets.append(returnPacket)

    return packets*/
