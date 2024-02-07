// /******************************************************************************
//  * @brief
//  *
//  * @file RoveCommUDP.h
//  * @author Eli Byrd (edbgkk@mst.edu)
//  * @date 2024-02-06
//  *
//  * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
//  ******************************************************************************/

// #ifndef ROVECOMM_UDP_H
// #define ROVECOMM_UDP_H

// #include <arpa/inet.h>
// #include <csignal>
// #include <cstring>
// #include <functional>
// #include <iostream>
// #include <netinet/in.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <vector>

// #include "RoveCommConsts.h"
// #include "RoveCommManifest.h"
// #include "RoveCommPacket.h"

// #include "../interfaces/AutonomyThread.hpp"

// class RoveCommUDP : public AutonomyThread<void>
// {
//     private:
//         int udpSocket;

//         struct sockaddr_in udpServerAddr;

//         std::vector<std::tuple<std::function<void(const RoveCommPacket&, const sockaddr_in&)>, uint16_t>> udpCallbacks;

//     public:
//         // Constructor
//         RoveCommUDP() : udpSocket(-1) {}

//         // Destructor
//         ~RoveCommUDP();

//         // Initialize the UDP TCP sockets
//         bool InitUDPSocket(const char* ipAddress, int port);

//         // Add a UDP callback
//         void AddUDPCallback(std::function<void(const RoveCommPacket&, const sockaddr_in&)> callback, const uint16_t& condition);

//         // Receive a UDP packet and call the appropriate callback
//         void ReceiveUDPPacketAndCallback();

//         // Send a UDP packet
//         ssize_t SendUDPPacket(const RoveCommPacket& data, const char* ipAddress, int port);

//         // Receive a UDP packet
//         ssize_t ReceiveUDPPacket(char* buffer, size_t bufferSize, sockaddr_in& clientAddr);

//         // Close the UDP socket
//         void CloseUDPSocket();

//         // AutonomyThread methods
//         void ThreadedContinuousCode() override;
//         void PooledLinearCode() override;
// };

// #endif    // ROVECOMM_UDP_H
