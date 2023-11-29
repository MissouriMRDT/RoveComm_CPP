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
        RoveCommEthernetTcp(RoveCommServerPort port) : m_unPort(port){};

        void Init() override;
        void Shutdown() override;

        int Write(RoveCommPacket& packet) const override;
        int WriteTo(RoveCommPacket& packet, RoveCommAddress address) const override;
        std::vector<RoveCommPacket>& Read() const override;

        // std::map<std::string, int> m_OpenSockets;
        // std::map<std::string, int> m_IncomingSockets;
        // std::map<int, int> m_Buffers;
        // int m_ServerFd;
        // int m_nOpenSocketLength;
        // int m_nIncomingSocketLength;

        // RoveCommEthernetTcp();
        // RoveCommEthernetTcp(std::string szHost /* = "127.0.0.1"*/, int nPort /*= ROVECOMM_TCP_PORT*/);

        // void CloseSockets();
        // int Write(RoveCommPacket& Packet);
        // int Connect(sockaddr stAddress);
        // void HandleIncomingConnection();
        // RoveCommPacket* Read();
    private:
        unsigned int m_unPort;
        int m_ServerFd;
        std::map<std::string, int> m_OpenSockets;
        std::map<std::string, int> m_IncomingSockets;
        std::map<int, int> m_Buffers;
        int m_nOpenSocketLength;
        int m_nIncomingSocketLength;
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
