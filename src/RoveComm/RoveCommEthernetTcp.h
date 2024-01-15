/******************************************************************************
 * @brief RoveCommEthernetTcp Implementation for RoveComm C++ Library
 *
 * @file RoveCommEthernetTcp.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-10-08
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_ETHERNET_TCP_H
#define ROVECOMM_ETHERNET_TCP_H

#include "RoveCommConstants.h"
#include "RoveCommPacket.h"
#include "RoveCommServer.h"

#include <iostream>
#include <map>
#include <string>
#include <sys/socket.h>
#include <vector>

using RoveCommSocket = int;    // kind of stupid why is this a thing

/******************************************************************************
 * @brief Extends RoveCommServer to use the TCP protocol. Both hosts must establish a
 * connection first. TCP guarantees no data corruption and successful delivery of data
 * in the same order in which it was sent.
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-14
 ******************************************************************************/
class RoveCommEthernetTcp : RoveCommServer

{
    public:
        /******************************************************************************
         * @brief Construct a new TCP server. Uses host IPv4 address
         *
         * @param unPort - port to listen on
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-12-21
         ******************************************************************************/
        RoveCommEthernetTcp(RoveCommPort unPort) : RoveCommServer(unPort){};

        void Init() override;
        void Shutdown() override;

        int Write(const RoveCommPacket& packet) override;
        int SendTo(const RoveCommPacket& packet, RoveCommAddress address) override;
        std::vector<const RoveCommPacket> Read() override;

        /******************************************************************************
         * @brief Try to open a TCP connection with another device (acting as client)
         * This method is mostly used internally.
         *
         * @param address The address to connect to
         * @return bool - Whether the connection was successful.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-12-21
         ******************************************************************************/
        bool Connect(const RoveCommAddress& address);

        /******************************************************************************
         * @brief Check for other devices trying to connect to this device (acting as server)
         * This will be private in a future iteration ;)
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-12-21
         ******************************************************************************/
        void AcceptIncomingConnections();

    private:
        void _register_socket(const RoveCommAddress& sAddress, RoveCommSocket nSocket);
        void _unregister_socket(const RoveCommAddress& sAddress, RoveCommSocket nSocket);

    private:
        // Socket for accepting connections from other devices
        RoveCommSocket m_nListeningSocket;
        // Connections opened by other devices (still 2-way!)
        // Note to self: in Python RoveComm, incoming_sockets are tracked separately,
        // and outgoing messages only get broadcasted to incoming_socket's
        std::map<RoveCommAddress, RoveCommSocket> m_mOpenSockets;
        // Buffers to persist incomplete recv()'s
        std::map<RoveCommSocket, std::vector<char>> m_mReadBuffers;

        // fd_set's contain all sockets for interfacing with the c library

        // This is kept precariously up to date with m_mOpenSockets
        // For selecting sockets to read
        fd_set m_sReadSet;
        // Cache the max socket in m_sReadSet
        int m_nMaxSocket = 0;
        // For selecting sockets to accept
        fd_set m_sAcceptSet;
};

/******************************************************************************
 * @brief I love java so much! When calling RoveCommEthernetTCP::Connect(),
 * remember to try{}catch(auto& exception) { exception.Print(); }
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-12-21
 ******************************************************************************/
class RoveCommTcpConnectionFailedException
{
    public:
        void Print();

        inline const int GetErrorCode() const { return m_nErrorCode; }

    private:
        const int m_nErrorCode;
};

#endif    // ROVECOMM_ETHERNET_TCP_H

// old comments
/*
    The TCP implementation for RoveComm.

    Methods:
    --------
        write(packet):
            Transmits a packet to the destination IP and all active subscribers.
        read():
            Receives all TCP packets from open sockets and packs data into RoveCommPacket instances
        connect(ip_octet):
            Opens a socket connection to the given address
        close_sockets():
            Closes the server socket and all open sockets
        handle_incoming_connections():
            Accepts socket connection requests
*/
