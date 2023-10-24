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
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

RoveCommEthernetTcp::RoveCommEthernetTcp()
{
    return;
}

RoveCommEthernetTcp::RoveCommEthernetTcp(std::string szHost /*= "127.0.0.1"*/, int nPort /*= ROVECOMM_TCP_PORT*/)
{
    m_OpenSockets     = {};
    m_IncomingSockets = {};
    m_Buffers         = {};

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

void RoveCommEthernetTcp::CloseSockets()
{
    for (const auto& nCurrentPair : m_OpenSockets)
    {
        int nCurrentFd = nCurrentPair.second;

        // Notifies other end that we are terminating the connection
        shutdown(nCurrentFd, 1);
        close(nCurrentFd);
    }
    return;
}

int RoveCommEthernetTcp::Write(RoveCommPacket& Packet)
{
    try
    {
        /*
        rovecomm_packet = struct.pack(
            ROVECOMM_HEADER_FORMAT,
            ROVECOMM_VERSION,
            packet.data_id,
            packet.data_count,
            types_byte_to_int[packet.data_type],
        )
        */
        for (int i = 0; i < Packet.m_nDataCount; i++)
        {
            // rovecomm_packet += struct.pack(">" + Packet.m_cDataType, i);
            continue;
        }
        for (const auto& nCurrentPair : m_IncomingSockets)
        {
            int nCurrentFd = nCurrentPair.second;

            // Notifies other end that we are terminating the connection
            // send(nCurrentFd, ?, ?, ?); - should send packed RoveCommPacket
        }
        if (true)    //(Packet.m_stIp != ("0.0.0.0", 0))
        {
            if (Connect(Packet.m_stIp.szIp) == 0)
            {
                return 0;
            }
            // send(m_ServerFd, m_OpenSockets[Packet.m_stIp.szIp], ?, ?); - should send packed RoveCommPacket
            // Establish a new connection if the destination has not yet been connected to yet
            return 1;
        }
    }
    catch (...)
    {
        return 0;
    }
}

int RoveCommEthernetTcp::Connect(std::string szAddress)
{
    if (!m_OpenSockets.contains(szAddress))
    {
        int nTCPSocketFd = socket(-1, SOCK_STREAM, -1);
        try
        {
            // Rather than a string, szHost should be a pointer to a sockaddr struct
            // connect(nTCPSocketFd, szAddress);
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
    return;
}

RoveCommPacket* RoveCommEthernetTcp::Read()    // needs to return pointer to array of RoveCommPackets
{
    RoveCommPacket* rcReturn = new RoveCommPacket();

    return rcReturn;
}
