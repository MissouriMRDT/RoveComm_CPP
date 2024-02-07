/******************************************************************************
 * @brief
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

#include "RoveCommConsts.h"
#include "RoveCommManifest.h"
#include "RoveCommPacket.h"

#include "../interfaces/AutonomyThread.hpp"

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
     * @brief The vector of TCP callbacks.
     *
     * @tparam T - The type of data that is to be sent or received. This can be
     *             any of the types defined in the manifest.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    template<typename T>
    std::vector<std::tuple<std::function<void(const RoveCommPacket<T>&, int)>, uint16_t>> vTCPCallbacks;

    /******************************************************************************
     * @brief
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    class RoveCommTCP : public AutonomyThread<void>
    {
        private:
            int nTCPSocket;

            struct sockaddr_in saTCPServerAddr;

            // AutonomyThread methods
            void ThreadedContinuousCode() override;
            void PooledLinearCode() override;

        public:
            // Constructor
            RoveCommTCP() : nTCPSocket(-1) {}

            // Destructor
            ~RoveCommTCP();

            // Initialize the TCP sockets
            bool InitTCPSocket(const char* cIPAddress, int nPort);

            // Add a TCP callback
            template<typename T>
            void AddTCPCallback(std::function<void(const RoveCommPacket<T>&, int)> fnCallback, const uint16_t& unCondition);

            // Receive a TCP packet and call the appropriate callback
            template<typename T>
            void ReceiveTCPPacketAndCallback(int nClientSocket);

            // Send a TCP packet
            template<typename T>
            ssize_t SendTCPPacket(int nClientSocket, const RoveCommPacket<T>& stData);

            // Receive a TCP packet
            ssize_t ReceiveTCPPacket(int nClientSocket, char* cBuffer, size_t siBufferSize);

            // Accept a TCP connection
            int AcceptTCPConnection();

            // Close the TCP socket
            void CloseTCPSocket();
    };
}    // namespace rovecomm

#endif    // ROVECOMM_TCP_H
