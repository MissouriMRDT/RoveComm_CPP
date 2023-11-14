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
#include "RoveCommServer.h"

#include <sys/socket.h>

/******************************************************************************
 * @brief Extends RoveCommServer to use the UDP protocol. UDP is a fast connectionless transport
 *   protocol that guarantees no data corruption but does not guarantee delivery,
 *   and if it delivers does not guarantee it being in the same order it was sent.
 *
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-14
 ******************************************************************************/
class RoveCommEthernetUdp : RoveCommServer
{
    public:
        void init() override;
        void shutdown() override;

        void write(RoveCommPacket& packet) const override;
        RoveCommPacket& read() const override;

        // int m_nRoveCommPort;
        // int m_nNumSubscribers;
        // sockaddr* m_aSubscribers[10];
        // int m_RoveCommSocketFd;

        // RoveCommEthernetUdp(int nPort = rovecomm::ROVECOMM_UDP_PORT);

        // int Subscribe(std::string szSubToIp);
        // int Write(const RoveCommPacket& Packet);
        // RoveCommPacket* Read();
        // void CloseSocket();
};

#endif    // ROVECOMM_ETHERNET_UDP_H

// old comments
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
