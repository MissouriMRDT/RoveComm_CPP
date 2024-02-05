/******************************************************************************
 * @brief Main Header for RoveComm C++
 *
 * @file RoveComm.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-07-29
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_H
#define ROVECOMM_H

#include "RoveCommPacket.h"
#include <arpa/inet.h>
#include <csignal>
#include <cstring>
#include <functional>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class RoveComm
{
    private:
        int udpSocket;
        int tcpSocket;

        struct sockaddr_in udpServerAddr;
        struct sockaddr_in tcpServerAddr;

        std::vector<std::tuple<std::function<void(const RoveCommPacket&, const sockaddr_in&)>, uint16_t>> udpCallbacks;
        std::vector<std::tuple<std::function<void(const RoveCommPacket&, int)>, uint16_t>> tcpCallbacks;

    public:
        RoveComm() : udpSocket(-1), tcpSocket(-1) {}

        bool initUDPSocket(const char* ipAddress, int port);

        void addUDPCallback(std::function<void(const RoveCommPacket&, const sockaddr_in&)> callback, const uint16_t& condition);

        bool initTCPSocket(const char* ipAddress, int port);

        void addTCPCallback(std::function<void(const RoveCommPacket&, int)> callback, const uint16_t& condition);

        void receiveUDPPacketAndCallback();

        void receiveTCPPacketAndCallback(int clientSocket);

        ssize_t sendUDPPacket(const RoveCommPacket& data, const char* ipAddress, int port);

        ssize_t sendTCPPacket(int clientSocket, const RoveCommPacket& data);

        ssize_t receiveUDPPacket(char* buffer, size_t bufferSize, sockaddr_in& clientAddr);

        ssize_t receiveTCPPacket(int clientSocket, char* buffer, size_t bufferSize);

        int acceptTCPConnection();

        void closeTCPSocket();

        void closeUDPSocket();

        ~RoveComm();
};

#endif    // ROVECOMM_H
