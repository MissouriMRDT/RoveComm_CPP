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

#include <arpa/inet.h>
#include <csignal>
#include <cstring>
#include <functional>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_set>
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
     * @brief The RoveCommUDP class is used to send and receive data over a UDP
     *        connection.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    class RoveCommUDP : AutonomyThread<void>
    {
        private:
            int nUDPSocket;
            struct sockaddr_in saUDPServerAddr;
            std::vector<SubscriberInfo> vSubscribers;

            template<typename T>
            void ProcessPacket(const RoveCommData& stData,
                               const std::vector<std::tuple<std::function<void(const RoveCommPacket<T>&, const sockaddr_in&)>, uint32_t>>& vCallbacks,
                               const sockaddr_in& saClientAddr);

            void ReceiveUDPPacketAndCallback();

            void AddSubscriber(const std::string& szIPAddress, const int& nPort);
            void RemoveSubscriber(const std::string& szIPAddress, const int& nPort);

            void ThreadedContinuousCode() override;
            void PooledLinearCode() override;

        public:
            RoveCommUDP() : nUDPSocket(-1) {}

            bool InitUDPSocket(int nPort);

            template<typename T>
            ssize_t SendUDPPacket(const RoveCommPacket<T>& stData, const char* cIPAddress, int nPort);

            template<typename T>
            void AddUDPCallback(std::function<void(const RoveCommPacket<T>&, const sockaddr_in&)> fnCallback, const uint16_t& unCondition);

            template<typename T>
            void RemoveUDPCallback(std::function<void(const RoveCommPacket<T>&, const sockaddr_in&)> fnCallback);

            void CloseUDPSocket();

            ~RoveCommUDP();

            // NOTE: These functions are for testing purposes only and should not be used in production code!
            template<typename T>
            void CallProcessPacket(const RoveCommData& stData,
                                   const std::vector<std::tuple<std::function<void(const RoveCommPacket<T>&, const sockaddr_in&)>, uint32_t>>& vCallbacks,
                                   const sockaddr_in& saClientAddr)
            {
                ProcessPacket(stData, vCallbacks, saClientAddr);
            }
    };

}    // namespace rovecomm

#endif    // ROVECOMM_UDP_H
