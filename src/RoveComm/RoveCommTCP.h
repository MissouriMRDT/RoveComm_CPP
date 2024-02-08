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

#include <arpa/inet.h>
#include <csignal>
#include <cstring>
#include <functional>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "../interfaces/AutonomyThread.hpp"
#include "ExternalIncludes.h"
#include "RoveCommConsts.h"
#include "RoveCommGlobals.h"
#include "RoveCommManifest.h"
#include "RoveCommPacket.h"

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
            int nTCPSocket;
            struct sockaddr_in saTCPServerAddr;

            template<typename T>
            void ProcessPacket(const RoveCommData& stData, const std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<T>&)>, uint16_t>>& vCallbacks);

            void ReceiveTCPPacketAndCallback();

            void ThreadedContinuousCode() override;
            void PooledLinearCode() override;

        public:
            RoveCommTCP() : nTCPSocket(-1) {}

            bool InitTCPSocket(const char* cIPAddress, int nPort);

            template<typename T>
            ssize_t SendTCPPacket(const RoveCommPacket<T>& stData, const char* cClientIPAddress, int nClientPort);

            template<typename T>
            void AddTCPCallback(std::function<void(const RoveCommPacket<T>&)> fnCallback, const uint16_t& unCondition);

            template<typename T>
            void RemoveTCPCallback(std::function<void(const RoveCommPacket<T>&)> fnCallback);

            void CloseTCPSocket();

            ~RoveCommTCP();
    };
}    // namespace rovecomm

#endif    // ROVECOMM_TCP_H
