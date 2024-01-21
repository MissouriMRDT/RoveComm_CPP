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

#include "RoveCommConstants.h"
#include "RoveCommHelpers.h"
#include "RoveCommPacket.h"
#include "RoveCommServer.h"

#include <list>
#include <netinet/in.h>
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
class RoveCommEthernetUdp : public RoveCommServer
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
        RoveCommEthernetUdp(RoveCommPort unPort) : RoveCommServer(unPort){};

        void Init() override;
        void Shutdown() override;

        int Write(const RoveCommPacket& packet) override;
        int SendTo(const RoveCommPacket& packet, RoveCommAddress address) override;
        std::vector<RoveCommPacket> Read() override;

        // TODO: subscribe/unsubscribe functions
        // void Subscribe(const RoveCommAddress& address);
        // void Unsubscribe(const RoveCommAddress& address);

    private:
        RoveCommSocket m_nSocket;
        fd_set m_sReadSet;
        // these aren't meant to be read outside the class, so I'm being lazy and using the native struct type
        std::list<sockaddr_in> m_lSubscribers;

        // TODO: consider caching outgoing sendto()'s with connect() and a connections list?
};

#endif    // ROVECOMM_ETHERNET_UDP_H
