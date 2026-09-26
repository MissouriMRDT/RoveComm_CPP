/******************************************************************************
 * @brief The RoveCommTCP class is used to send and receive data over a TCP
 *        connection. This class is a subclass of AutonomyThread, so it can be
 *        run in its own thread.
 *
 * @file RoveCommTCP.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-06
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_TCP_H
#define ROVECOMM_TCP_H

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
     * @brief The RoveCommTCP class is used to send and receive data over a TCP
     *        connection.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    class RoveCommTCP : AutonomyThread<void>
    {
        private:
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
            std::atomic_int m_nTCPSocket;
            struct sockaddr_in m_saTCPServerAddr;
            std::atomic_int m_nCurrentTCPClientSocket;
            struct sockaddr_in m_saClientAddr;
            std::shared_mutex m_muCallbackMutex;
            std::mutex m_muSocketSendMutex;

#if defined(__ROVECOMM_TESTS_MODE__) && __ROVECOMM_TESTS_MODE__ == 1
        public:
#endif

            // Packet processing functions
            template<typename T>
            void ProcessPacket(std::span<const uint8_t> stData);
            void ReceiveAndCallback();

        private:
            // AutonomyThread member functions
            void ThreadedContinuousCode() override;
            void PooledLinearCode() override;

        public:
            // Constructor
            RoveCommTCP();
            // Destructor
            ~RoveCommTCP();

            // Initialization
            bool Init(const manifest::AddressEntry& stIPAddress = {0, 0, 0, 0}, int nPort = manifest::General::ETHERNET_TCP_PORT);

            // Data transmission
            template<typename T>
            ssize_t Send(const RoveCommPacket<T>& stData, const manifest::AddressEntry& stClientIPAddress, int nClientPort);

            template<typename manifest::ManifestEntry Entry>
            ssize_t Send(std::span<const EntryType<Entry>> spData,
                         const manifest::AddressEntry& stIPAddress = manifest::Helpers::FindBoardById(Entry.DATA_ID).ADDRESS,
                         int nPort                                 = manifest::General::ETHERNET_TCP_PORT)
            {
                return Send(rovecomm::CreatePacket<Entry>(spData), stIPAddress, nPort);
            }

            template<typename manifest::ManifestEntry Entry>
            ssize_t Send(std::initializer_list<EntryType<Entry>> ilData,
                         const manifest::AddressEntry& stIPAddress = manifest::Helpers::FindBoardById(Entry.DATA_ID).ADDRESS,
                         int nPort                                 = manifest::General::ETHERNET_TCP_PORT)
            {
                return Send(rovecomm::CreatePacket<Entry>(ilData), stIPAddress, nPort);
            }

            // Callback management
            template<typename T>
            CallbackHandle On(const uint16_t unDataId, std::function<void(const RoveCommPacket<T>&)> fnCallback);

            template<typename manifest::ManifestEntry Entry>
            CallbackHandle On(std::function<void(const RoveCommPacket<EntryType<Entry>>&)> fnCallback)
            {
                return On(Entry.DATA_ID, fnCallback);
            }

            void Off(const CallbackHandle& stHandle);

            template<typename T>
            void Clear(const uint16_t unDataId);

            template<typename manifest::ManifestEntry Entry>
            void Clear()
            {
                Clear<typename manifest::Helpers::RoveCommToCType<Entry.DATA_TYPE>::c_type>(Entry.DATA_ID);
            }

            // Deinitialization
            void Close();

            // Selectively make inherited method public so we can get RoveCommNode FPS.
            using AutonomyThread::GetIPS;
    };
}    // namespace rovecomm

#endif    // ROVECOMM_TCP_H
