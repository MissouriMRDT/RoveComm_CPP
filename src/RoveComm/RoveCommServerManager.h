/******************************************************************************
 * @brief Network thread management.
 *
 * @file RoveCommPacket.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-10-08
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_SERVER_MANAGER_H
#define ROVECOMM_SERVER_MANAGER_H

#include <chrono>
#include <deque>
#include <functional>
#include <future>
#include <iostream>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <vector>

#include "LibraryImports.h"
#include "RoveCommHelpers.h"
#include "RoveCommPacket.h"
#include "RoveCommServer.h"

/******************************************************************************
 * @brief Handles lifetimes of RoveCommServer's and defer work to them in a thread-safe manner.
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
         * @param unPort - Which port to open on
         * @param protocol - Which protocol to use
         * @return bool - Whether the server initialized correctly
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-24
         ******************************************************************************/
        bool OpenServerOnPort(uint16_t unPort, RoveCommProtocol protocol = UDP);
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

        // /******************************************************************************
        //  * @brief Pop the oldest packet off the queue.
        //  * Example usage:
        //  * if(auto packet = RoveComm.Next()) {...}
        //  *
        //  * @return std::optional<const RoveCommPacket> - the potential packet
        //  *
        //  * @author OcelotEmpire (hobbz.pi@gmail.com)
        //  * @date 2024-01-19
        //  ******************************************************************************/
        // std::optional<const RoveCommPacket> NextPacket();

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
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-20
         ******************************************************************************/
        void SetCallback(uint16_t unId, std::function<void(const RoveCommPacket&)> fCallback /*, bool bRemoveFromQueue = true*/);
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
                // packet queue has been removed for now. I'll leave this struct in case we have other fields to store in it.
                // bool bRemoveFromQueue;                                 // whether to remove from packet queue after invocation
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
        // std::deque<RoveCommPacket> m_dqPacketQueue;                   // packets
        std::deque<std::function<void()>> m_dqCommandQueue;    // write, sendto, subscribeto
        // std::deque<RoveCommFetchRequest> m_dqRequestQueue;

        std::mutex m_muServerStateMutex;    // TODO: make it so each server has its own lock
        // std::mutex m_muPacketQueueMutex;
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
