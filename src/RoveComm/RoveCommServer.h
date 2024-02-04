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

#include <chrono>
#include <deque>
#include <functional>
#include <future>
#include <iostream>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <vector>

#include "RoveCommHelpers.h"
#include "RoveCommPacket.h"
#include "ThreadHack.h"

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

/******************************************************************************
 * @brief Handles lifetimes of RoveCommServer's and defer work to them
 *
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-14
 ******************************************************************************/
class RoveCommServerManager : public AutonomyThread<void>
{
    public:
        static RoveCommServerManager& GetInstance()
        {
            static RoveCommServerManager instance;
            return instance;
        }

    private:
        RoveCommServerManager() {}

    public:
        /******************************************************************************
         * @brief Initialize RoveComm
         *
         * @return bool - whether RoveComm initialized correctly
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-24
         ******************************************************************************/
        bool Init();
        /******************************************************************************
         * @brief Attempt to open a new server
         *
         * @param port - Which port to open on
         * @param protocol - Which protocol to use
         * @return bool - Whether the server initialized correctly
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-24
         ******************************************************************************/
        bool OpenServerOnPort(uint16_t port, RoveCommProtocol protocol = UDP);
        /******************************************************************************
         * @brief Stop netowrk thread and close up RoveComm
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-24
         ******************************************************************************/
        void Shutdown();
        /******************************************************************************
         * @brief Write a packet to the server
         *
         * @param packet - the packet to write
         * @param protocol - which protocol to use
         * @return int - number of sockets which sent successfully, 0 if failed
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-24
         ******************************************************************************/
        void Write(const RoveCommPacket& packet, RoveCommProtocol protocol = UDP);
        /******************************************************************************
         * @brief Send a packet to a specific IP
         *
         * @param packet - packet to send
         * @param address - the address to send to
         * @param protocol - the protocol to use
         * @return int - how many bytes were actually sent
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-24
         ******************************************************************************/
        void SendTo(const RoveCommPacket& packet, const RoveCommAddress& address, RoveCommProtocol protocol = UDP);

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
        std::optional<const RoveCommPacket> NextPacket();

        // static inline std::deque<RoveCommPacket>& GetPacketQueue() { return m_dqPacketQueue; }

        /******************************************************************************
         * @brief Ensure that a copy of all packets sent from the given address are sent to this device.
         *
         * @param address Device from which to request packets
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-02-03
         ******************************************************************************/
        void SubscribeTo(const RoveCommAddress& address, RoveCommProtocol protocol = UDP);

        /******************************************************************************
         * @brief Wait for RoveCommPacket with specified data id and/or address
         *
         * @param unId - the data id to listen for (default: rovecomm::System::ANY_DATA_ID)
         * @param address - the address to listen for (default: RoveCommAddress::ANY_ADDRESS)
         * @param ulTimeout - milliseconds to wait before giving up
         * @return std::shared_future<RoveCommPacket> - use future.get() or future.wait() to block thread until response
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-19
         ******************************************************************************/
        // std::shared_future<RoveCommPacket> Fetch(RoveCommDataId unId          = rovecomm::System::ANY_DATA_ID,
        //                                          RoveCommAddress address      = RoveCommAddress::ANY_ADDRESS,
        //                                          unsigned long long ulTimeout = 0);
        /******************************************************************************
         * @brief Wait for first RoveCommPacket that satisfies the given predicate
         *
         * @param fFilter - function that returns true if the given packet, address is a match
         * @param ulTimeout - milliseconds to wait before giving up
         * @return std::shared_future<RoveCommPacket> -
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-02-03
         ******************************************************************************/
        // std::shared_future<RoveCommPacket> Fetch(std::function<bool(const RoveCommPacket&, const RoveCommAddress&)> fFilter, unsigned long long ulTimeout = 0);

        /******************************************************************************
         * @brief Specify a function to invoke when a certain data ID is received.
         * This may invoke asynchronously, so watch your mutexes!
         * Using Fetch() is recommended.
         *
         * @param unId - Data ID to listen for, rovecomm::System::ANY will trigger for all packets, i.e. "default" callback
         * @param fCallback - The function to execute.
         * @param bRemoveFromQueue - Whether to remove the packet from the queue after execution so that it won't be picked up by NextPacket()
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-20
         ******************************************************************************/
        void SetCallback(uint16_t unId, std::function<void(const RoveCommPacket&)> fCallback, bool bRemoveFromQueue = true);
        /******************************************************************************
         * @brief Remove a callback function
         *
         * @param unId - Data ID to listen for, rovecomm::System::ANY will only remove the "default" callback if previously set, NOT all callbacks.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-02-03
         ******************************************************************************/
        void ClearCallback(uint16_t unId);

    private:
        struct RoveCommCallback
        {
                std::function<void(const RoveCommPacket&)> fInvoke;    // function to invoke
                bool bRemoveFromQueue;                                 // whether to remove from packet queue after invocation
        };

        // struct RoveCommFetchRequest
        // {
        //         std::function<bool(const RoveCommPacket&, const RoveCommAddress&)> fPredicate;    // for filtering packets
        //         std::promise<RoveCommPacket> pmPromise;                                           // consumed by std::shared_future's throughout the autonomy codebase
        //         std::chrono::time_point<std::chrono::system_clock> tmCreated;                     // time at which the packet was created
        //         std::chrono::milliseconds tmTimeout;                                              // milliseconds until the packet removes itself, 0 if no timeout
        // };

        // only one server per protocol type for now
        std::map<RoveCommProtocol, RoveCommServer*> m_mServers;

        std::map<uint16_t, RoveCommCallback> m_mCallbacks;            // data_id -> callback
        std::deque<std::function<void()>> m_dqCallbackInvokeQueue;    // data_id's to process
        static const int s_nMaxCallbackThreads = 4;                   // number of threads for PooledLinearCode()
        std::deque<RoveCommPacket> m_dqPacketQueue;                   // packets
        std::deque<std::function<void()>> m_dqCommandQueue;           // write, sendto, subscribeto
        // std::deque<RoveCommFetchRequest> m_dqRequestQueue;

        std::mutex m_muServerStateMutex;    // TODO: make it so each server has its own lock
        std::mutex m_muPacketQueueMutex;
        std::mutex m_muCommandQueueMutex;
        std::shared_mutex m_muCallbackListMutex;
        std::mutex m_muCallbackInvokeMutex;

        void ThreadedContinuousCode() override;
        void PooledLinearCode() override;
        void _write(const RoveCommPacket& packet, RoveCommProtocol protocol);
        void _send_to(const RoveCommPacket& packet, const RoveCommAddress& address, RoveCommProtocol protocol);
        void _subscribe_to(const RoveCommAddress& address, RoveCommProtocol protocol);
};

#endif
