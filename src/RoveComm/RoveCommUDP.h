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
#include "RoveCommGlobals.h"
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

            template<typename T>
            struct RoveCommRegistry
            {
                    static inline std::unordered_map<uint16_t, std::vector<std::function<void(const rovecomm::RoveCommPacket<T>&)>>> umCallbackMap;
            };

            // Private member variables
            std::atomic_int m_nUDPSocket;
            struct sockaddr_in m_saUDPServerAddr;
            using SubscriberInfo = std::pair<std::string, int>;
            std::set<SubscriberInfo> seSubscribers;
            std::shared_mutex m_muCallbackMutex;
            std::mutex m_muSocketSendMutex;
            std::mutex m_muSocketReceiveMutex;

#ifdef BUILD_TESTS_MODE
        public:
#endif

            // Packet processing functions
            template<typename T>
            void ProcessPacket(std::span<const uint8_t> stData, const sockaddr_in& saClientAddr);
            void ReceiveAndCallback();

            // Subscriber management functions
            void AddSubscriber(const std::string& szIPAddress, const int nPort);
            void RemoveSubscriber(const std::string& szIPAddress, const int nPort);

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
            bool Init(int nPort = 11000);

            // Data transmission functions
            template<typename T>
            ssize_t Send(const RoveCommPacket<T>& stPacket, const std::string& szIPAddress, int nPort);

            // template<typename T>
            // ssize_t Send(const RoveCommPacket<T>& stPacket, const std::string& szIPAddress);

            // Callback management functions

            template<typename T>
            void On(const uint16_t unDataId, std::function<void(const RoveCommPacket<T>&)> fnCallback);

            // template<typename T>
            // void On(const std::string& szBoardName, const std::string& szPacketName, std::function<void(const RoveCommPacket<T>&)> fnCallback);

            template<typename T>
            void Clear(const uint16_t unDataId);

            // template<typename T>
            // void Clear(const std::string& szBoardName, const std::string& szPacketName);

            // Deinitialization
            void Close();

            // Selectively make inherited method public so we can get RoveCommNode FPS.
            using AutonomyThread::GetIPS;
    };

}    // namespace rovecomm

#endif    // ROVECOMM_UDP_H
