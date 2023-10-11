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
#include "Consts.h"
#include "RoveCommPacket.h"
#include "string"
#include <map>
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
        std::map<std::string, socklen_t> m_OpenSockets;
        std::map<std::string, socklen_t> m_IncomingSockets;
        std::map<std::string, int> m_Buffers;
        socklen_t Server;

        RoveCommEthernetTcp(std::string szHost, int nPort = ROVECOMM_TCP_PORT);

        void CloseSockets();
        int Write(RoveCommPacket& Packet);
        int Connect(std::string szAddress);
        void HandleIncomingConnection();
        RoveCommPacket[] & Read();
}

#endif    // ROVECOMM_ETHERNET_TCP_H
