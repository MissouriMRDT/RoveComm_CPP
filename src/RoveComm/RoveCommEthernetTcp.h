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

#include <map>
#include <string>
#include <sys/socket.h>

class RoveCommEthernetTcp
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
{
    public:
        std::map<std::string, int> m_OpenSockets;
        std::map<std::string, int> m_IncomingSockets;
        std::map<int, int> m_Buffers;
        int m_ServerFd;
        int m_nOpenSocketLength;
        int m_nIncomingSocketLength;

        RoveCommEthernetTcp();
        RoveCommEthernetTcp(std::string szHost /* = "127.0.0.1"*/, int nPort /*= ROVECOMM_TCP_PORT*/);

        void CloseSockets();
        int Write(RoveCommPacket& Packet);
        int Connect(sockaddr stAddress);
        void HandleIncomingConnection();
        RoveCommPacket* Read();
};

#endif    // ROVECOMM_ETHERNET_TCP_H
