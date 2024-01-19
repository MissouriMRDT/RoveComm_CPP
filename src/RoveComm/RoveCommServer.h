/******************************************************************************
 * @brief Base class for network thread management.
 *
 * @file RoveCommPacket.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-10-08
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_SERVER_H
#define ROVECOMM_SERVER_H

// #include <future>
#include <iostream>
#include <map>
// #include <queue>
// #include <shared_mutex>
#include <thread>
#include <vector>

#include "RoveCommHelpers.h"
#include "RoveCommPacket.h"

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
        // RoveCommServer(RoveCommPort unPort) : m_unPort(unPort) {}
        RoveCommServer(RoveCommPort unPort) : m_unPort(unPort) {}

        virtual ~RoveCommServer();

        /******************************************************************************
         * @brief Initialize listening socket and start thread
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-11-29
         ******************************************************************************/
        virtual void Init();
        /******************************************************************************
         * @brief Close all open sockets and shut down thread
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-11-29
         ******************************************************************************/
        virtual void Shutdown();

        /******************************************************************************
         * @brief Write a packet to all subscribers/connections
         *
         * @param packet the RoveCommPacket to write
         * @return int how many sockets sent successfully
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-11-29
         ******************************************************************************/
        virtual int Write(const RoveCommPacket& packet);
        /******************************************************************************
         * @brief Write a packet to a single address, but not subscribers/connections.
         * If connection does not already exist, the server will attempt to establish a new connection.
         * If you want to send a packet to subscribers AND the ip, call Subscribe()
         *
         * @param packet the RoveCommPacket to write
         * @param address the RoveCommAddress (IP, Port) to send to
         * @return int the number of bytes written (0 if error)
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-11-29
         ******************************************************************************/
        virtual int SendTo(const RoveCommPacket& packet, RoveCommAddress address);
        /******************************************************************************
         * @brief Read incoming packets and clear queue
         *
         * @return std::vector<const RoveCommPacket> a list of RoveCommPackets. size() == 0 if no packets available.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-11-29
         ******************************************************************************/
        virtual std::vector<const RoveCommPacket> Read();

        // /******************************************************************************
        //  * @brief Synchronously await the next RoveCommPacket with the given data_id.
        //  * This packet is marked immediately as read and does not get queued for Read()
        //  *
        //  * @param unId the desired id
        //  * @param address the desired address
        //  * @return std::future<RoveCommPacket> the desired RoveCommPacket
        //  *
        //  * @author OcelotEmpire (hobbz.pi@gmail.com)
        //  * @date 2023-12-01
        //  ******************************************************************************/
        // virtual std::future<RoveCommPacket> Fetch(RoveCommDataId unId = rovecomm::System::ANY, RoveCommAddress address = RoveCommAddress::ANY) const;

        inline const RoveCommPort GetPort() const { return m_unPort; }

    protected:
        const RoveCommPort m_unPort;
        // std::map<RoveCommDataId, RoveCommCallback> m_fCallbacks;
        // std::thread m_thNetworkThread;
        // std::queue<RoveCommPacket> m_qPacketCopyQueue;
        // std::shared_mutex m_muPoolScheduleMutex;
        // std::mutex m_muPacketCopyMutex;
};

/******************************************************************************
 * @brief Handles lifetimes of RoveCommServer's and defer work to them
 *
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-14
 ******************************************************************************/
class RoveCommServerManager
{
    public:
        static void Init();
        static void OpenServerOnPort(RoveCommPort port, RoveCommProtocol protocol = RoveCommProtocol::UDP);
        static void Shutdown();
        static int Write(RoveCommPacket& packet, RoveCommProtocol protocol = RoveCommProtocol::UDP);
        static int SendTo(RoveCommPacket& packet, RoveCommAddress address, RoveCommProtocol protocol = RoveCommProtocol::UDP);
        static std::vector<RoveCommPacket> Read(RoveCommProtocol protocol = RoveCommProtocol::UDP);

        // static std::future<RoveCommPacket> Fetch(RoveCommDataId unId = rovecomm::System::ANY, RoveCommAddress address = RoveCommAddress::ANY);
        static void SetCallback(RoveCommDataId unId, RoveCommCallback fCallback);

    private:
        static std::map<RoveCommProtocol, std::vector<RoveCommServer*>> s_Servers;
        static std::map<RoveCommDataId, RoveCommCallback> s_fCallbacks;

        static std::thread s_thNetworkThread;
};

// This is the main singleton that we call static functions on
// So we can get Java-style RoveComm.write(...);
extern RoveCommServerManager RoveComm;    // TODO: make this a real singleton

#endif

// Old comments
// // FIXME: Change parameter variables names to meet our style guide.
// RoveComm(int udp_port, int tcp_addr);

// // FIXME: Change function names and paramerter variable names to meet our style guide.
// static void listen();
// static void set_callback(int data_id, std::string& func);    // Find way to pass function as input
// static void clear_callback(int data_id);
// static void set_default_callback(std::string& func);         // Find way to pass function as input
// static void clear_default_callback();
// static int write(RoveCommPacket& packet, bool reliable = false);
// static void close_thread();
