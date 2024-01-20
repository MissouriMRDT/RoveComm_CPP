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
#include <deque>
#include <future>
#include <iostream>
#include <map>
#include <mutex>
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

        RoveCommServer(RoveCommServer&& other) = delete;

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
         * @return int the number of bytes written (0 if error or failure)
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
        virtual std::vector<RoveCommPacket> Read();

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

        inline RoveCommPort GetPort() const { return m_unPort; }

    protected:
        const RoveCommPort m_unPort;
};

struct RoveCommCallback
{
        std::function<void(RoveCommPacket)> fInvoke;
        bool bRemoveFromQueue;
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
        static void OpenServerOnPort(RoveCommPort port, RoveCommProtocol protocol = UDP);
        static void Shutdown();
        static int Write(RoveCommPacket& packet, RoveCommProtocol protocol = UDP);
        static int SendTo(RoveCommPacket& packet, RoveCommAddress address, RoveCommProtocol protocol = UDP);

        /******************************************************************************
         * @brief Pop the oldest packet off the queue.
         * Example usage:
         * if(auto packet = RoveComm.Next()) {...}
         *
         * @return std::optional<const RoveCommPacket> - the potential packet
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-19
         ******************************************************************************/
        static std::optional<const RoveCommPacket> Next();

        // static inline std::deque<RoveCommPacket>& GetPacketQueue() { return s_dqPacketQueue; }

        // use Fetch() instead
        //  static std::vector<RoveCommPacket> Read(RoveCommProtocol protocol = UDP);

        /******************************************************************************
         * @brief Wait for RoveCommPacket
         *
         * @param unId -
         * @param address -
         * @return std::future<RoveCommPacket> -
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-19
         ******************************************************************************/
        static std::future<RoveCommPacket> Fetch(RoveCommDataId unId = rovecomm::System::ANY, RoveCommAddress address = RoveCommAddress::ANY);

        /******************************************************************************
         * @brief Specify a function to invoke when a certain data id is received
         *
         * @param unId - Id to listen for, rovecomm::System::ANY will trigger for all packets.
         * @param fCallback - The function to execute.
         * @param bRemoveFromQueue - Whether to remove the packet from the queue after execution.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-20
         ******************************************************************************/
        static void SetCallback(RoveCommDataId unId, std::function<void(RoveCommPacket)> fCallback, bool bRemoveFromQueue = true);
        static void ClearCallback(RoveCommDataId unId);

    private:
        static void _read_all_to_queue();
        static void _loop_func();

    private:
        // only one server per protocol type for now
        static std::map<RoveCommProtocol, RoveCommServer*> s_mServers;
        // only one callback per dataId
        static std::map<RoveCommDataId, RoveCommCallback> s_mCallbacks;
        static std::deque<RoveCommPacket> s_dqPacketQueue;

        static bool bStopThread;
        static std::thread s_thNetworkThread;
        static std::mutex s_muQueueMutex;
};

// This is the main singleton that we call static functions on
// So we can get Java-style RoveComm.write();
extern RoveCommServerManager RoveComm;

// TODO: make this a real singleton ^^^

#endif
