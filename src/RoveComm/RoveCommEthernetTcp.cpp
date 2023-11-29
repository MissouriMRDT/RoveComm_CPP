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
#include <iostream>
#include <memory>
#include <mutex>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

RoveCommEthernetTcp::init(std::string szHost /*= "127.0.0.1"*/, int nPort /*= ROVECOMM_TCP_PORT*/)
{
    m_OpenSockets           = {};
    m_IncomingSockets       = {};
    m_Buffers               = {};
    m_nOpenSocketLength     = 0;
    m_nIncomingSocketLength = 0;

    // socket() returns file descriptor and takes domain, type, and protocol ints, same as a Python socket
    // While a socket object does not exist, SocketFd can be used to refer to the created socket in socket functions
    m_ServerFd  = socket(AF_INET, SOCK_STREAM, -1);

    int nOptVal = 1;

    // Allows the socket address to be reused after being closed
    setsockopt(m_ServerFd, SOL_SOCKET, SO_REUSEADDR, &nOptVal, 4);    // last argument is byte length of OptVal

    // Fixes an error on linux with opening the socket again too soon
    try
    {
        setsockopt(m_ServerFd, SOL_SOCKET, SO_REUSEPORT, &nOptVal, 4);
    }
    catch (...)
    {}

    // Bind the socket to the current machine's local network IP by default(can be specified as well)
    // Rather than a string, szHost should be a pointer to a sockaddr struct
    // bind(m_ServerFd, &szHost, nPort);

    // Accept up to 5 simulataneous connections, before we start discarding them
    listen(m_ServerFd, 5);
}

void RoveCommEthernetTcp::shutdown()
{
    for (const auto& nCurrentPair : m_OpenSockets)
    {
        int nCurrentFd = nCurrentPair.second;

        // Notifies other end that we are terminating the connection
        shutdown(nCurrentFd, 1);
        close(nCurrentFd);
        m_nOpenSocketLength -= 1;
    }

    return;
}

int RoveCommEthernetTcp::write(RoveCommPacket& Packet) const
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
            send(m_OpenSockets[Packet.m_stIp.stIp.sa_data], &szRoveCommPacket, sizeof(szRoveCommPacket), 0);
            // Establish a new connection if the destination has not yet been connected to yet
            return 1;
        }
    }
    catch (...)
    {
        return 0;
    }
}

int RoveCommEthernetTcp::Connect(sockaddr stAddress)
{
    if (!m_OpenSockets.contains(stAddress.sa_data))
    {
        int nTCPSocketFd = socket(-1, SOCK_STREAM, -1);
        try
        {
            // Rather than a string, szHost should be a pointer to a sockaddr struct
            connect(nTCPSocketFd, &stAddress, 4);
        }
        catch (...)
        {
            // Log exception
            return 0;
        }
        return 1;
    }
    return 1;
}

void RoveCommEthernetTcp::HandleIncomingConnection()
{
    sockaddr connection;
    socklen_t length;
    int conn                              = accept(m_ServerFd, &connection, &length);
    m_OpenSockets[connection.sa_data]     = conn;
    m_IncomingSockets[connection.sa_data] = conn;
    m_nOpenSocketLength += 1;
    m_nIncomingSocketLength += 1;
    return;
}

RoveCommPacket* RoveCommEthernetTcp::Read()    // needs to return pointer to array of RoveCommPackets
{
    int aAvailableSockets[5];
    int packetLength = 0;
    // FIX ME: Add an actual length to packets array
    RoveCommPacket packets[5];
    int currentIndex         = 0;
    RoveCommPacket* rcReturn = new RoveCommPacket();
    for (const auto& nCurrentPair : m_OpenSockets)
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
