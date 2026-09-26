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
            CallbackMap<T>& GetCallbackMap()
            {
                return std::get<CallbackMap<T>>(m_tpCallbackMaps);
            }

            template<typename T>
            void RemoveCallback(const CallbackHandle& stHandle);

            // Private member variables
            std::atomic_int m_nUDPSocket;
            struct sockaddr_in m_saUDPServerAddr;
            using SubscriberInfo = std::pair<manifest::AddressEntry, int>;
            std::set<SubscriberInfo> seSubscribers;
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

            // Subscriber management functions
            void AddSubscriber(const std::string& stIPAddress, const int nPort);
            void RemoveSubscriber(const std::string& stIPAddress, const int nPort);

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

            // Data transmission functions
            template<typename T>
            ssize_t Send(const RoveCommPacket<T>& stPacket, const manifest::AddressEntry& stIPAddress = {0, 0, 0, 0}, int nPort = manifest::General::ETHERNET_UDP_PORT);

            template<typename manifest::ManifestEntry Entry>
            ssize_t Send(std::span<const EntryType<Entry>> spData,
                         const manifest::AddressEntry& stIPAddress = manifest::Helpers::FindBoardById(Entry.DATA_ID).ADDRESS,
                         int nPort                                 = manifest::General::ETHERNET_UDP_PORT)
            {
                return Send(rovecomm::CreatePacket<Entry>(spData), stIPAddress, nPort);
            }

            template<typename manifest::ManifestEntry Entry>
            ssize_t Send(std::initializer_list<EntryType<Entry>> ilData,
                         const manifest::AddressEntry& stIPAddress = manifest::Helpers::FindBoardById(Entry.DATA_ID).ADDRESS,
                         int nPort                                 = manifest::General::ETHERNET_UDP_PORT)
            {
                return Send(rovecomm::CreatePacket<Entry>(ilData), stIPAddress, nPort);
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
                return Send(RoveCommPacket<uint8_t>{.unDataId = manifest::System::SUBSCRIBE_DATA_ID, .vData = {}}, stIPAddress, nPort);
            }

            ssize_t Unsubscribe(const manifest::AddressEntry& stIPAddress, int nPort = manifest::General::ETHERNET_UDP_PORT)
            {
                return Send(RoveCommPacket<uint8_t>{.unDataId = manifest::System::UNSUBSCRIBE_DATA_ID, .vData = {}}, stIPAddress, nPort);
            }

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
