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
        void _register_socket(const RoveCommAddress& sAddress, RoveCommSocket nSocket, bool bIsIncoming);
        void _unregister_socket(const RoveCommAddress& sAddress);

    private:
        // Socket for accepting connections from other devices
        RoveCommSocket m_nListeningSocket;
        // All open connections (outgoing and incoming)
        std::map<RoveCommAddress, RoveCommSocket> m_mOpenSockets;
        // The sockets that Write() will send() to
        std::map<RoveCommAddress, RoveCommSocket> m_mIncomingSockets;
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

#endif    // ROVECOMM_ETHERNET_TCP_H