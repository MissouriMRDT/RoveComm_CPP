// /******************************************************************************
//  * @brief
//  *
//  * @file RoveCommUDP.cpp
//  * @author Eli Byrd (edbgkk@mst.edu)
//  * @date 2024-02-06
//  *
//  * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
//  ******************************************************************************/

// #include "RoveCommUDP.h"

// /******************************************************************************
//  * @brief
//  *
//  * @param ipAddress -
//  * @param port -
//  * @return true -
//  * @return false -
//  *
//  * @author Eli Byrd (edbgkk@mst.edu)
//  * @date 2024-02-06
//  ******************************************************************************/
// bool RoveCommUDP::InitUDPSocket(const char* ipAddress, int port)
// {
//     udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
//     if (udpSocket == -1)
//     {
//         std::cerr << "Error creating UDP socket\n";
//         return false;
//     }

//     memset(&udpServerAddr, 0, sizeof(udpServerAddr));
//     udpServerAddr.sin_family      = AF_INET;
//     udpServerAddr.sin_addr.s_addr = inet_addr(ipAddress);
//     udpServerAddr.sin_port        = htons(port);

//     if (bind(udpSocket, (struct sockaddr*) &udpServerAddr, sizeof(udpServerAddr)) == -1)
//     {
//         std::cerr << "Error binding UDP socket\n";
//         close(udpSocket);
//         return false;
//     }

//     return true;
// }

// /******************************************************************************
//  * @brief
//  *
//  * @param callback -
//  * @param condition -
//  *
//  * @author Eli Byrd (edbgkk@mst.edu)
//  * @date 2024-02-06
//  ******************************************************************************/
// void RoveCommUDP::AddUDPCallback(std::function<void(const RoveCommPacket&, const sockaddr_in&)> callback, const uint16_t& condition)
// {
//     udpCallbacks.push_back(std::make_tuple(callback, condition));
// }

// /******************************************************************************
//  * @brief
//  *
//  *
//  * @author Eli Byrd (edbgkk@mst.edu)
//  * @date 2024-02-06
//  ******************************************************************************/
// void RoveCommUDP::ReceiveUDPPacketAndCallback()
// {
//     char udpBuffer[sizeof(RoveCommPacket)];
//     sockaddr_in clientAddr;
//     ssize_t udpBytesReceived = ReceiveUDPPacket(udpBuffer, sizeof(udpBuffer), clientAddr);

//     if (udpBytesReceived == sizeof(RoveCommPacket))
//     {
//         RoveCommPacket receivedStruct;
//         memcpy(&receivedStruct, udpBuffer, sizeof(RoveCommPacket));

//         std::cout << "Received UDP" << std::endl;

//         for (const auto& callbackInfo : udpCallbacks)
//         {
//             auto& callback  = std::get<0>(callbackInfo);
//             auto& condition = std::get<1>(callbackInfo);

//             if (condition == uint16_t(receivedStruct.data_id))
//             {
//                 callback(receivedStruct, clientAddr);
//             }
//         }
//     }
// }

// /******************************************************************************
//  * @brief
//  *
//  * @param data -
//  * @param ipAddress -
//  * @param port -
//  * @return ssize_t -
//  *
//  * @author Eli Byrd (edbgkk@mst.edu)
//  * @date 2024-02-06
//  ******************************************************************************/
// ssize_t RoveCommUDP::SendUDPPacket(const RoveCommPacket& data, const char* ipAddress, int port)
// {
//     struct sockaddr_in udpClientAddr;
//     memset(&udpClientAddr, 0, sizeof(udpClientAddr));
//     udpClientAddr.sin_family = AF_INET;
//     udpClientAddr.sin_port   = htons(port);
//     inet_pton(AF_INET, ipAddress, &udpClientAddr.sin_addr);

//     return sendto(udpSocket, &data, sizeof(RoveCommPacket), 0, (struct sockaddr*) &udpClientAddr, sizeof(udpClientAddr));
// }

// /******************************************************************************
//  * @brief
//  *
//  * @param buffer -
//  * @param bufferSize -
//  * @param clientAddr -
//  * @return ssize_t -
//  *
//  * @author Eli Byrd (edbgkk@mst.edu)
//  * @date 2024-02-06
//  ******************************************************************************/
// ssize_t RoveCommUDP::ReceiveUDPPacket(char* buffer, size_t bufferSize, sockaddr_in& clientAddr)
// {
//     socklen_t addrLen = sizeof(clientAddr);
//     return recvfrom(udpSocket, buffer, bufferSize, 0, (struct sockaddr*) &clientAddr, &addrLen);
// }

// /******************************************************************************
//  * @brief
//  *
//  *
//  * @author Eli Byrd (edbgkk@mst.edu)
//  * @date 2024-02-06
//  ******************************************************************************/
// void RoveCommUDP::CloseUDPSocket()
// {
//     close(udpSocket);
// }

// /******************************************************************************
//  * @brief
//  *
//  *
//  * @author Eli Byrd (edbgkk@mst.edu)
//  * @date 2024-02-06
//  ******************************************************************************/
// void RoveCommUDP::ThreadedContinuousCode() {}

// /******************************************************************************
//  * @brief
//  *
//  *
//  * @author Eli Byrd (edbgkk@mst.edu)
//  * @date 2024-02-06
//  ******************************************************************************/
// void RoveCommUDP::PooledLinearCode() {}

// /******************************************************************************
//  * @brief Destroy the RoveCommUDP::RoveCommUDP object.
//  *
//  *
//  * @author Eli Byrd (edbgkk@mst.edu)
//  * @date 2024-02-06
//  ******************************************************************************/
// RoveCommUDP::~RoveCommUDP()
// {
//     if (udpSocket != -1)
//     {
//         CloseUDPSocket();
//     }
// }
