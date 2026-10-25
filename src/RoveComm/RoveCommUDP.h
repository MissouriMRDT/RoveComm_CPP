/******************************************************************************
 * @brief The RoveCommUDP class is used to send and receive data over a UDP
 *        connection.
 *
 * @file RoveCommUDP.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-06
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_UDP_H
#define ROVECOMM_UDP_H

#include "ExternalIncludes.h"
#include "RoveCommConsts.h"
#include "RoveCommManifest.h"
#include "RoveCommPacket.h"

/// \cond
#include <atomic>
#include <csignal>
#include <cstring>
#include <functional>
#include <iostream>
#include <shared_mutex>
#include <set>
#include <tuple>
#include <unordered_map>
#include <vector>

/// \endcond

/******************************************************************************
 * @brief The RoveComm namespace contains all of the functionality for the
 *        RoveComm library. This includes the packet structure and the
 *        functions for packing and unpacking data.
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-07
 ******************************************************************************/
namespace rovecomm
{
    /******************************************************************************
     * @brief The RoveCommUDP class is used to send and receive data over a UDP
     *        connection.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    class RoveCommUDP : AutonomyThread<void>
    {
        private:
#if defined(__ROVECOMM_WINDOWS_MODE__) && __ROVECOMM_WINDOWS_MODE__ == 1
            // Windows specific private member variables.
            HANDLE m_stdIOCP;
#endif

            // One callback registered with On(). The ID lets Off() remove exactly this callback.
            template<typename T>
            struct CallbackEntry
            {
                    uint64_t ullID;
                    std::function<void(const RoveCommPacket<T>&)> fnCallback;
            };

            // This node's callbacks for one payload type, by data ID.
            template<typename T>
            using CallbackMap = std::unordered_map<uint16_t, std::vector<CallbackEntry<T>>>;

            // This node's callbacks, one map per payload type. They belong to this instance, so two nodes never
            // share callbacks, and m_muCallbackMutex guards every map.
            std::tuple<CallbackMap<int8_t>,
                       CallbackMap<uint8_t>,
                       CallbackMap<int16_t>,
                       CallbackMap<uint16_t>,
                       CallbackMap<int32_t>,
                       CallbackMap<uint32_t>,
                       CallbackMap<float>,
                       CallbackMap<double>,
                       CallbackMap<char>>
                m_tpCallbackMaps;
            uint64_t m_ullNextCallbackID = 1;    // Guarded by m_muCallbackMutex. 0 marks an empty CallbackHandle.

            template<typename T>
            constexpr CallbackMap<T>& GetCallbackMap()
            {
                return std::get<CallbackMap<T>>(m_tpCallbackMaps);
            }

            template<typename T>
            void RemoveCallback(const CallbackHandle& stHandle);

            // Private member variables
            std::atomic_int m_nUDPSocket;
            struct sockaddr_in m_saUDPServerAddr;
            using SubscriberInfo = std::pair<manifest::AddressEntry, int>;
            std::set<SubscriberInfo> m_seSubscribers;
            std::shared_mutex m_muCallbackMutex;
            std::mutex m_muSocketSendMutex;
            std::mutex m_muSocketReceiveMutex;

#if defined(__ROVECOMM_TESTS_MODE__) && __ROVECOMM_TESTS_MODE__ == 1
        public:
#endif

            // Packet processing functions
            template<typename T>
            void ProcessPacket(std::span<const uint8_t> stData, const sockaddr_in& saClientAddr);
            void ReceiveAndCallback();

        private:
            // AutonomyThread member functions
            void ThreadedContinuousCode() override;
            void PooledLinearCode() override;

        public:
            // Constructor
            RoveCommUDP();
            // Destructor
            ~RoveCommUDP();

            // Initialization
            bool Init(int nPort = manifest::General::ETHERNET_UDP_PORT);

            // Data transmission
            template<typename T>
            ssize_t WriteAll(const RoveCommPacket<T>& stPacket, std::span<const std::pair<manifest::AddressEntry, int>> spDestinations);

            /******************************************************************************
             * @brief Sends a packet to a specific destination. This is often used to send a command to a specific device.
             *
             * @tparam T - The payload type of the packet. Must be one of the manifest's types.
             * @param stPacket - The packet to be sent.
             * @param stIPAddress - The IP address of the destination.
             * @param nPort - The port number of the destination.
             * @return ssize_t - The number of bytes sent, or -1 if an error occurred.
             *
             * @author OcelotEmpire (hobbz.pi@gmail.com)
             * @date 2026-10-23
             ******************************************************************************/
            template<typename T>
            ssize_t WriteTo(const RoveCommPacket<T>& stPacket, const manifest::AddressEntry& stIPAddress, int nPort = manifest::General::ETHERNET_UDP_PORT)
            {
                std::pair<manifest::AddressEntry, int> stdDestination{stIPAddress, nPort};
                return WriteAll(stPacket, std::span<const std::pair<manifest::AddressEntry, int>>{&stdDestination, 1});
            }

            /******************************************************************************
             * @brief Sends a packet to all subscribers. This is often used to send errors or telemetry data to all devices that have subscribed to this node.
             *
             * @tparam T - The payload type of the packet. Must be one of the manifest's types.
             * @param stPacket - The packet to be sent.
             * @return ssize_t - The number of bytes sent, or -1 if an error occurred.
             *
             * @author OcelotEmpire (hobbz.pi@gmail.com)
             * @date 2026-10-23
             ******************************************************************************/
            template<typename T>
            ssize_t Write(const RoveCommPacket<T>& stPacket)
            {
                // std::set is non-contiguous, so we need to copy the subscribers into a contiguous vector before passing it to WriteAll().
                // In C++26 we will have std::flat_set, which will allow us to avoid this copy.
                std::vector<std::pair<manifest::AddressEntry, int>> vDestinations(m_seSubscribers.begin(), m_seSubscribers.end());
                return WriteAll(stPacket, vDestinations);
            }

            /******************************************************************************
             * @brief Writes a packet, checked against the manifest
             *
             * @tparam Entry -
             * @param spData -
             * @return ssize_t -
             *
             * @author OcelotEmpire (hobbz.pi@gmail.com)
             * @date 2026-10-23
             ******************************************************************************/
            template<typename manifest::ManifestEntry Entry>
            ssize_t Write(std::span<const EntryType<Entry>> spData)
            {
                return Write(rovecomm::CreatePacket<Entry>(spData));
            }

            template<typename manifest::ManifestEntry Entry>
            ssize_t Write(std::initializer_list<EntryType<Entry>> ilData)
            {
                // Necessary because std::initializer_list cannot be deduced as std::span for some stupid reason: https://isocpp.org/files/papers/P4144R1.html
                return Write(rovecomm::CreatePacket<Entry>(ilData));
            }

            template<typename manifest::ManifestEntry Entry>
            ssize_t WriteTo(std::span<const EntryType<Entry>> spData,
                            const manifest::AddressEntry& stIPAddress = manifest::Helpers::FindBoardById(Entry.DATA_ID).ADDRESS,
                            int nPort                                 = manifest::General::ETHERNET_UDP_PORT)
            {
                return WriteTo(rovecomm::CreatePacket<Entry>(spData), stIPAddress, nPort);
            }

            template<typename manifest::ManifestEntry Entry>
            ssize_t WriteTo(std::initializer_list<EntryType<Entry>> ilData,
                            const manifest::AddressEntry& stIPAddress = manifest::Helpers::FindBoardById(Entry.DATA_ID).ADDRESS,
                            int nPort                                 = manifest::General::ETHERNET_UDP_PORT)
            {
                return WriteTo(rovecomm::CreatePacket<Entry>(ilData), stIPAddress, nPort);
            }

            // Callback management functions

            template<typename T>
            CallbackHandle On(const uint16_t unDataId, std::function<void(const RoveCommPacket<T>&)> fnCallback);

            template<typename manifest::ManifestEntry Entry>
            CallbackHandle On(std::function<void(const RoveCommPacket<EntryType<Entry>>&)> fnCallback)
            {
                return On(Entry.DATA_ID, fnCallback);
            }

            void Off(const CallbackHandle& stHandle);

            ssize_t Subscribe(const manifest::AddressEntry& stIPAddress, int nPort = manifest::General::ETHERNET_UDP_PORT)
            {
                return WriteTo(RoveCommPacket<uint8_t>{.unDataId = manifest::System::SUBSCRIBE_DATA_ID, .vData = {}}, stIPAddress, nPort);
            }

            ssize_t Unsubscribe(const manifest::AddressEntry& stIPAddress, int nPort = manifest::General::ETHERNET_UDP_PORT)
            {
                return WriteTo(RoveCommPacket<uint8_t>{.unDataId = manifest::System::UNSUBSCRIBE_DATA_ID, .vData = {}}, stIPAddress, nPort);
            }

            void Clear(const uint16_t unDataId);

            template<typename T>
            void Clear(const uint16_t unDataId);

            template<typename manifest::ManifestEntry Entry>
            void Clear()
            {
                Clear<EntryType<Entry>>(Entry.DATA_ID);
            }

            // Deinitialization
            void Close();

            // Selectively make inherited method public so we can get RoveCommNode FPS.
            using AutonomyThread::GetIPS;
    };

}    // namespace rovecomm

#endif    // ROVECOMM_UDP_H
