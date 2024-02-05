/******************************************************************************
 * @brief Main Header for RoveComm C++
 *
 * @file RoveComm.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-07-29
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#include "RoveComm.h"

bool RoveComm::initUDPSocket(const char* ipAddress, int port)
{
    udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocket == -1)
    {
        std::cerr << "Error creating UDP socket\n";
        return false;
    }

    memset(&udpServerAddr, 0, sizeof(udpServerAddr));
    udpServerAddr.sin_family      = AF_INET;
    udpServerAddr.sin_addr.s_addr = inet_addr(ipAddress);
    udpServerAddr.sin_port        = htons(port);

    if (bind(udpSocket, (struct sockaddr*) &udpServerAddr, sizeof(udpServerAddr)) == -1)
    {
        std::cerr << "Error binding UDP socket\n";
        close(udpSocket);
        return false;
    }

    return true;
}

void RoveComm::addUDPCallback(std::function<void(const RoveCommPacket&, const sockaddr_in&)> callback, const uint16_t& condition)
{
    udpCallbacks.push_back(std::make_tuple(callback, condition));
}

bool RoveComm::initTCPSocket(const char* ipAddress, int port)
{
    tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpSocket == -1)
    {
        std::cerr << "Error creating TCP socket\n";
        return false;
    }

    memset(&tcpServerAddr, 0, sizeof(tcpServerAddr));
    tcpServerAddr.sin_family      = AF_INET;
    tcpServerAddr.sin_addr.s_addr = inet_addr(ipAddress);
    tcpServerAddr.sin_port        = htons(port);

    if (bind(tcpSocket, (struct sockaddr*) &tcpServerAddr, sizeof(tcpServerAddr)) == -1)
    {
        std::cerr << "Error binding TCP socket\n";
        close(tcpSocket);
        return false;
    }

    if (listen(tcpSocket, 5) == -1)
    {
        std::cerr << "Error listening on TCP socket\n";
        close(tcpSocket);
        return false;
    }

    return true;
}

void RoveComm::addTCPCallback(std::function<void(const RoveCommPacket&, int)> callback, const uint16_t& condition)
{
    tcpCallbacks.push_back(std::make_tuple(callback, condition));
}

void RoveComm::receiveUDPPacketAndCallback()
{
    char udpBuffer[sizeof(RoveCommPacket)];
    sockaddr_in clientAddr;
    ssize_t udpBytesReceived = receiveUDPPacket(udpBuffer, sizeof(udpBuffer), clientAddr);

    if (udpBytesReceived == sizeof(RoveCommPacket))
    {
        RoveCommPacket receivedStruct;
        memcpy(&receivedStruct, udpBuffer, sizeof(RoveCommPacket));

        std::cout << "Received UDP" << std::endl;

        for (const auto& callbackInfo : udpCallbacks)
        {
            auto& callback  = std::get<0>(callbackInfo);
            auto& condition = std::get<1>(callbackInfo);

            if (condition == uint16_t(receivedStruct.data_id))
            {
                callback(receivedStruct, clientAddr);
            }
        }
    }
}

void RoveComm::receiveTCPPacketAndCallback(int clientSocket)
{
    char tcpBuffer[sizeof(RoveCommPacket)];
    ssize_t tcpBytesReceived = receiveTCPPacket(clientSocket, tcpBuffer, sizeof(tcpBuffer));

    if (tcpBytesReceived == sizeof(RoveCommPacket))
    {
        RoveCommPacket receivedStruct;
        memcpy(&receivedStruct, tcpBuffer, sizeof(RoveCommPacket));

        for (const auto& callbackInfo : tcpCallbacks)
        {
            auto& callback  = std::get<0>(callbackInfo);
            auto& condition = std::get<1>(callbackInfo);

            if (condition == uint16_t(receivedStruct.data_id))
            {
                callback(receivedStruct, clientSocket);
            }
        }
    }
}

ssize_t RoveComm::sendUDPPacket(const RoveCommPacket& data, const char* ipAddress, int port)
{
    struct sockaddr_in udpClientAddr;
    memset(&udpClientAddr, 0, sizeof(udpClientAddr));
    udpClientAddr.sin_family = AF_INET;
    udpClientAddr.sin_port   = htons(port);
    inet_pton(AF_INET, ipAddress, &udpClientAddr.sin_addr);

    return sendto(udpSocket, &data, sizeof(RoveCommPacket), 0, (struct sockaddr*) &udpClientAddr, sizeof(udpClientAddr));
}

ssize_t RoveComm::sendTCPPacket(int clientSocket, const RoveCommPacket& data)
{
    return send(clientSocket, &data, sizeof(RoveCommPacket), 0);
}

ssize_t RoveComm::receiveUDPPacket(char* buffer, size_t bufferSize, sockaddr_in& clientAddr)
{
    socklen_t addrLen = sizeof(clientAddr);
    return recvfrom(udpSocket, buffer, bufferSize, 0, (struct sockaddr*) &clientAddr, &addrLen);
}

ssize_t RoveComm::receiveTCPPacket(int clientSocket, char* buffer, size_t bufferSize)
{
    return recv(clientSocket, buffer, bufferSize, 0);
}

int RoveComm::acceptTCPConnection()
{
    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    return accept(tcpSocket, (struct sockaddr*) &clientAddr, &addrLen);
}

void RoveComm::closeTCPSocket()
{
    close(tcpSocket);
}

void RoveComm::closeUDPSocket()
{
    close(udpSocket);
}

RoveComm::~RoveComm()
{
    if (udpSocket != -1)
        closeUDPSocket();
    if (tcpSocket != -1)
        closeTCPSocket();
}
