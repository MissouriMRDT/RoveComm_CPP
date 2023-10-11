/******************************************************************************
 * @brief RoveCommEthernetUdp Implementation for RoveComm C++ Library
 *
 * @file RoveCommEthernetUdp.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-10-08
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_ETHERNET_UDP_H
#define ROVECOMM_ETHERNET_UDP_H
#include "Consts.h"
#include "RoveCommPacket.h"
#include <sys/socket.h>

class RoveCommEthernetUdp
/*
    The UDP implementation for RoveComm. UDP is a fast connectionless transport
    protocol that guarantees no data corruption but does not guarantee delivery,
    and if it delivers does not guarantee it being in the same order it was
    sent.

    Methods:
    --------
        write(packet):
            Transmits a packet to the destination IP and all active subscribers.
        read():
            Unpacks the UDP packet and packs it into a RoveComm Packet for easy
            parsing in other code.
        subscribe(ip_octet):
            Subscribes to UDP packets from the given ip
        close_socket():
            Closes the UDP socket
*/
{
    public:
        int m_nRoveCommPort;
        sockaddr* m_aSubscribers[10];
        socklen_t RoveCommSocket;
        RoveCommEthernetUdp(int nPort = ROVECOMM_UDP_PORT);

        int Subscribe(std::string szSubToIp);
        int Write(RoveCommPacket& Packet);
        RoveCommPacket& Read();
        void CloseSocket();
}

#endif    // ROVECOMM_ETHERNET_UDP_H
