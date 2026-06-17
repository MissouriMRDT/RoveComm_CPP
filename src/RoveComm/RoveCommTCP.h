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
            template<typename T>
            struct RoveCommRegistry
            {
                    static inline std::unordered_map<uint16_t, std::vector<std::function<void(const rovecomm::RoveCommPacket<T>&)>>> umCallbackMap{};
            };

            // Private member variables
            std::atomic_int m_nTCPSocket;
            struct sockaddr_in m_saTCPServerAddr;
            std::atomic_int m_nCurrentTCPClientSocket;
            struct sockaddr_in m_saClientAddr;
            std::shared_mutex m_muCallbackMutex;
            std::mutex m_muSocketSendMutex;

#ifdef BUILD_TESTS_MODE
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
            bool Init(const std::string& szIPAddress, int nPort = 12000);

            // Data transmission
            template<typename T>
            ssize_t Send(const RoveCommPacket<T>& stData, const std::string& szClientIPAddress, int nClientPort);

            // Callback management
            template<typename T>
            void On(const uint16_t unDataId, std::function<void(const RoveCommPacket<T>&)> fnCallback);

            template<typename T>
            void Clear(const uint16_t unDataId);

            // Deinitialization
            void Close();

            // Selectively make inherited method public so we can get RoveCommNode FPS.
            using AutonomyThread::GetIPS;
    };
}    // namespace rovecomm

#endif    // ROVECOMM_TCP_H
