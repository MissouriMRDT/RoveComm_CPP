/******************************************************************************
 * @brief
 *
 * @file RoveCommTCP.cpp
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-06
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#include "RoveCommTCP.h"

namespace rovecomm
{
    /******************************************************************************
     * @brief
     *
     * @param ipAddress -
     * @param port -
     * @return true -
     * @return false -
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-06
     ******************************************************************************/
    bool RoveCommTCP::InitTCPSocket(const char* cIPAddress, int nPort)
    {
        nTCPSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (nTCPSocket == -1)
        {
            std::cerr << "Error creating TCP socket\n";
            return false;
        }

        memset(&saTCPServerAddr, 0, sizeof(saTCPServerAddr));
        saTCPServerAddr.sin_family      = AF_INET;
        saTCPServerAddr.sin_addr.s_addr = inet_addr(cIPAddress);
        saTCPServerAddr.sin_port        = htons(nPort);

        if (bind(nTCPSocket, (struct sockaddr*) &saTCPServerAddr, sizeof(saTCPServerAddr)) == -1)
        {
            std::cerr << "Error binding TCP socket\n";
            close(nTCPSocket);
            return false;
        }

        if (listen(nTCPSocket, 5) == -1)
        {
            std::cerr << "Error listening on TCP socket\n";
            close(nTCPSocket);
            return false;
        }

        return true;
    }

    /******************************************************************************
     * @brief
     *
     * @param callback -
     * @param condition -
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-06
     ******************************************************************************/
    template<typename T>
    void RoveCommTCP::AddTCPCallback(std::function<void(const RoveCommPacket<T>&, int)> fnCallback, const uint16_t& unCondition)
    {
        vTCPCallbacks<T>.push_back(std::make_tuple(fnCallback, unCondition));
    }

    /******************************************************************************
     * @brief
     *
     * @param clientSocket -
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-06
     ******************************************************************************/
    template<typename T>
    void RoveCommTCP::ReceiveTCPPacketAndCallback(int nClientSocket)
    {
        char cTCPBuffer[sizeof(RoveCommPacket<T>)];
        ssize_t tcpBytesReceived = ReceiveTCPPacket(nClientSocket, cTCPBuffer, sizeof(cTCPBuffer));

        if (tcpBytesReceived == sizeof(RoveCommPacket<T>))
        {
            RoveCommPacket<T> stReceivedStruct;
            memcpy(&stReceivedStruct, cTCPBuffer, sizeof(RoveCommPacket<T>));

            for (const auto& callbackInfo : vTCPCallbacks<T>)
            {
                auto& callback  = std::get<0>(callbackInfo);
                auto& condition = std::get<1>(callbackInfo);

                if (condition == uint16_t(stReceivedStruct.unDataId))
                {
                    callback(stReceivedStruct, nClientSocket);
                }
            }
        }
    }

    /******************************************************************************
     * @brief
     *
     * @param clientSocket -
     * @param data -
     * @return ssize_t -
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-06
     ******************************************************************************/
    template<typename T>
    ssize_t RoveCommTCP::SendTCPPacket(int nClientSocket, const RoveCommPacket<T>& stData)
    {
        return send(nClientSocket, &stData, sizeof(RoveCommPacket<T>), 0);
    }

    /******************************************************************************
     * @brief
     *
     * @param clientSocket -
     * @param buffer -
     * @param bufferSize -
     * @return ssize_t -
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-06
     ******************************************************************************/
    ssize_t RoveCommTCP::ReceiveTCPPacket(int nClientSocket, char* cBuffer, size_t siBufferSize)
    {
        return recv(nClientSocket, cBuffer, siBufferSize, 0);
    }

    /******************************************************************************
     * @brief
     *
     * @return int -
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-06
     ******************************************************************************/
    int RoveCommTCP::AcceptTCPConnection()
    {
        struct sockaddr_in saClientAddr;
        socklen_t skAddrLen = sizeof(saClientAddr);
        return accept(nTCPSocket, (struct sockaddr*) &saClientAddr, &skAddrLen);
    }

    /******************************************************************************
     * @brief
     *
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-06
     ******************************************************************************/
    void RoveCommTCP::CloseTCPSocket()
    {
        close(nTCPSocket);
    }

    /******************************************************************************
     * @brief
     *
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-06
     ******************************************************************************/
    void RoveCommTCP::ThreadedContinuousCode() {}

    /******************************************************************************
     * @brief
     *
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-06
     ******************************************************************************/
    void RoveCommTCP::PooledLinearCode() {}

    /******************************************************************************
     * @brief Destroy the RoveCommTCP::RoveCommTCP object.
     *
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-06
     ******************************************************************************/
    RoveCommTCP::~RoveCommTCP()
    {
        if (nTCPSocket != -1)
        {
            CloseTCPSocket();
        }
    }
}    // namespace rovecomm
