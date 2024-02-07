/******************************************************************************
 * @brief Base class for system calls to sockets.
 *
 * @file RoveCommPacket.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-10-08
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_SERVER_H
#define ROVECOMM_SERVER_H

#include "RoveCommConstants.h"
#include "RoveCommHelpers.h"
#include "RoveCommPacket.h"

#include <vector>

/******************************************************************************
 * @brief Base class that can be extended to add more protocols.
 * Each server listens to ONE port on its own ONE network thread.
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-13
 ******************************************************************************/
class RoveCommServer
{
    public:
        RoveCommServer(uint16_t unPort) : m_usPort(unPort) {}

        // RoveCommServer(RoveCommPort unPort);

        RoveCommServer(RoveCommServer&& other) = delete;
        //
        virtual ~RoveCommServer() = default;

        /******************************************************************************
         * @brief Initialize listening socket and start thread
         *
         * @return bool - Whether the server initialized successfully
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-11-29
         ******************************************************************************/
        virtual bool Init() = 0;
        /******************************************************************************
         * @brief Close all open sockets and shut down thread
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-11-29
         ******************************************************************************/
        virtual void Shutdown() = 0;

        /******************************************************************************
         * @brief Write a packet to all subscribers/connections
         *
         * @param packet the RoveCommPacket to write
         * @return int how many sockets sent successfully
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-11-29
         ******************************************************************************/
        virtual int Write(const RoveCommPacket& packet) = 0;
        /******************************************************************************
         * @brief Write a packet to a single address, but not subscribers/connections.
         * If connection does not already exist, the server will attempt to establish a new connection.
         * If you want to send a packet to subscribers AND the ip, call Subscribe()
         *
         * @param packet the RoveCommPacket to write
         * @param address the RoveCommAddress (IP, Port) to send to
         * @return int the number of bytes written (0 if error or failure)
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-11-29
         ******************************************************************************/
        virtual int SendTo(const RoveCommPacket& packet, const RoveCommAddress& address) = 0;
        /******************************************************************************
         * @brief Read incoming packets and clear queue
         *
         * @return std::vector<const RoveCommPacket> a list of RoveCommPackets. size() == 0 if no packets available.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-11-29
         ******************************************************************************/
        virtual std::vector<RoveCommPacket> Read() = 0;
        /******************************************************************************
         * @brief Ensure that a copy of all packets sent from the given address are sent to this device.
         *
         * @param address Device from which to request packets
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-02-03
         ******************************************************************************/
        virtual void SubscribeTo(const RoveCommAddress& address) = 0;

        inline uint16_t GetPort() const { return m_usPort; }

        friend class RoveCommServerManager;

    protected:
        /******************************************************************************
         * @brief Optional method that RoveCommServerManager calls before each Read()
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-20
         ******************************************************************************/
        virtual void OnRoveCommUpdate() {}

    protected:
        const uint16_t m_usPort;
};

#endif
