/******************************************************************************
 * @brief RoveComm TCP class implementation.
 *
 * @file RoveCommTCP.cpp
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-07
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#include "RoveCommTCP.h"
#include "RoveCommPacket.h"

/// \cond
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>

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
     * @brief Construct a new RoveCommTCP::RoveCommTCP object.
     *
     *
     * @author clayjay3 (claytonraycowen@gmail.com)
     * @date 2024-03-07
     ******************************************************************************/
    RoveCommTCP::RoveCommTCP()
    {
        // Initialize member variables.
        m_nTCPSocket              = -1;
        m_nCurrentTCPClientSocket = -1;

        // Set an IPS cap in the backend RoveComm thread.
        this->SetMainThreadIPSLimit(rovecomm::ROVECOMM_THREAD_MAX_IPS);
    }

    /******************************************************************************
     * @brief Destroy the RoveCommTCP::RoveCommTCP object.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    RoveCommTCP::~RoveCommTCP()
    {
        CloseTCPSocket();
    }

    /******************************************************************************
     * @brief Initializes a TCP socket and binds it to the specified IP address and
     *        port. And then starts the threaded continuous code in AutonomyThread.
     *
     * @param cIPAddress - The IP address to bind the socket to. If set to "", the
     *                     socket will be bound to all available interfaces.
     * @param nPort - The port to bind the socket to. If set to 0, the OS will
     *                automatically assign an available port.
     * @return true - The TCP socket was successfully initialized and bound.
     * @return false - The TCP socket failed to initialize and bind.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    bool RoveCommTCP::InitTCPSocket(const char* cIPAddress, int nPort)
    {
        // Create a TCP socket
        m_nTCPSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_nTCPSocket == -1)
        {
            perror("Failed to create TCP socket");
            return false;
        }
        else
        {
            // Attempt to set the socket to non-blocking mode.
            if (fcntl(m_nTCPSocket, F_SETFL, fcntl(m_nTCPSocket, F_GETFL) | O_NONBLOCK) == -1)
            {
                // Handle and print error.
                perror("Failed to set UDP socket to non-blocking mode.");
                return false;
            }
        }

        // Configure the server address
        memset(&m_saTCPServerAddr, 0, sizeof(m_saTCPServerAddr));
        m_saTCPServerAddr.sin_family      = AF_INET;
        m_saTCPServerAddr.sin_addr.s_addr = inet_addr(cIPAddress);
        m_saTCPServerAddr.sin_port        = htons(nPort);

        // Bind the socket to the server address
        if (bind(m_nTCPSocket.load(), (struct sockaddr*) &m_saTCPServerAddr, sizeof(m_saTCPServerAddr)) == -1)
        {
            perror("Failed to bind TCP socket");
            close(m_nTCPSocket);
            return false;
        }

        // Listen for incoming connections
        if (listen(m_nTCPSocket, 5) == -1)
        {
            perror("Failed to listen on TCP socket");
            close(m_nTCPSocket);
            return false;
        }

        // Start the threaded continuous code
        Start();

        return true;
    }

    /******************************************************************************
     * @brief Sends a TCP packet to the specified client IP address and port.
     *        The data is packed into a RoveCommData struct and sent over the
     *        TCP socket.
     *
     * @tparam T - The data type of the RoveCommPacket. Must be one of the
     *             following: uint8_t, int8_t, uint16_t, int16_t, uint32_t,
     *             int32_t, float, double, or char.
     * @param stData - The RoveCommPacket to send over the TCP socket.
     * @param cClientIPAddress - The IP address of the client to send the packet to.
     * @param nClientPort - The port of the client to send the packet to.
     * @return ssize_t - The number of bytes sent over the TCP socket. Returns -1
     *                   if an error occurred.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    template<typename T>
    ssize_t RoveCommTCP::SendTCPPacket(const RoveCommPacket<T>& stPacket, const char* cClientIPAddress, int nClientPort)
    {
        // Create a TCP socket
        int nClientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (nClientSocket == -1)
        {
            perror("Failed to create TCP client socket");
            return -1;
        }

        // Configure the client address
        struct sockaddr_in saClientAddr;
        memset(&saClientAddr, 0, sizeof(saClientAddr));
        saClientAddr.sin_family = AF_INET;
        saClientAddr.sin_port   = htons(nClientPort);

        // Convert IP address to binary form
        if (inet_pton(AF_INET, cClientIPAddress, &saClientAddr.sin_addr) <= 0)
        {
            perror("Invalid address/ Address not supported");
            close(nClientSocket);
            return -1;
        }

        // Connect to the client
        if (connect(nClientSocket, (struct sockaddr*) &saClientAddr, sizeof(saClientAddr)) == -1)
        {
            perror("Connection failed");
            close(nClientSocket);
            return -1;
        }

        // Pack the data
        RoveCommData stData = PackPacket(stPacket);

        // Send the data
        ssize_t siBytesSent = send(nClientSocket, &stData, sizeof(stData), 0);
        // Check if any bytes were sent.
        if (siBytesSent == -1)
        {
            // Handle and print error message.
            perror("Failed to send data to TCP client socket");
        }

        // Close the client socket
        close(nClientSocket);

        return siBytesSent;
    }

    /******************************************************************************
     * @brief Adds a callback function to the vector of TCP callbacks for the
     *        specified data type. The callback function will be invoked when a
     *        packet with the specified data id is received.
     *
     * @tparam T - The data type of the RoveCommPacket. Must be one of the
     *             following: uint8_t, int8_t, uint16_t, int16_t, uint32_t,
     *             int32_t, float, double, or char.
     * @param fnCallback - The callback function to add to the vector of TCP
     *                     callbacks.
     * @param unCondition - The data id of the packet that will invoke the
     *                      callback function.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    template<typename T>
    void RoveCommTCP::AddTCPCallback(std::function<void(const RoveCommPacket<T>&)> fnCallback, const uint16_t& unCondition)
    {
        // Acquire a write lock to protect the callback vectors.
        std::unique_lock<std::shared_mutex> lkCallbackLock(m_muCallbackMutex);

        // Add the callback function to the vector of TCP callbacks for the specified data type
        if constexpr (std::is_same_v<T, uint8_t>)
        {
            // Add the callback function to the vector of uint8_t callbacks
            tcp::vUInt8Callbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
        else if constexpr (std::is_same_v<T, int8_t>)
        {
            // Add the callback function to the vector of int8_t callbacks
            tcp::vInt8Callbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
        else if constexpr (std::is_same_v<T, uint16_t>)
        {
            // Add the callback function to the vector of uint16_t callbacks
            tcp::vUInt16Callbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
        else if constexpr (std::is_same_v<T, int16_t>)
        {
            // Add the callback function to the vector of int16_t callbacks
            tcp::vInt16Callbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
        else if constexpr (std::is_same_v<T, uint32_t>)
        {
            // Add the callback function to the vector of uint32_t callbacks
            tcp::vUInt32Callbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
        else if constexpr (std::is_same_v<T, int32_t>)
        {
            // Add the callback function to the vector of int32_t callbacks
            tcp::vInt32Callbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
        else if constexpr (std::is_same_v<T, float>)
        {
            // Add the callback function to the vector of float callbacks
            tcp::vFloatCallbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            // Add the callback function to the vector of double callbacks
            tcp::vDoubleCallbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
        else if constexpr (std::is_same_v<T, char>)
        {
            // Add the callback function to the vector of char callbacks
            tcp::vCharCallbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
    }

    /******************************************************************************
     * @brief Removes a callback function from the vector of TCP callbacks for the
     *        specified data type. The callback function will no longer be invoked
     *        when a packet with the specified data id is received.
     *
     * @tparam T - The data type of the RoveCommPacket. Must be one of the
     *             following: uint8_t, int8_t, uint16_t, int16_t, uint32_t,
     *             int32_t, float, double, or char.
     * @param fnCallback - The callback function to remove from the vector of TCP
     *                     callbacks.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    template<typename T>
    void RoveCommTCP::RemoveTCPCallback(std::function<void(const RoveCommPacket<T>&)> fnCallback)
    {
        // Acquire a write lock to protect the callback vectors.
        std::unique_lock<std::shared_mutex> lkCallbackLock(m_muCallbackMutex);

        // Remove the callback function from the appropriate vector based on the data type T
        if constexpr (std::is_same_v<T, uint8_t>)
        {
            // Remove the callback function from the vector of uint8_t callbacks
            tcp::vUInt8Callbacks.erase(std::remove_if(tcp::vUInt8Callbacks.begin(),
                                                      tcp::vUInt8Callbacks.end(),
                                                      [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                       tcp::vUInt8Callbacks.end());
        }
        else if constexpr (std::is_same_v<T, int8_t>)
        {
            // Remove the callback function from the vector of int8_t callbacks
            tcp::vInt8Callbacks.erase(std::remove_if(tcp::vInt8Callbacks.begin(),
                                                     tcp::vInt8Callbacks.end(),
                                                     [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                      tcp::vInt8Callbacks.end());
        }
        else if constexpr (std::is_same_v<T, uint16_t>)
        {
            // Remove the callback function from the vector of uint16_t callbacks
            tcp::vUInt16Callbacks.erase(std::remove_if(tcp::vUInt16Callbacks.begin(),
                                                       tcp::vUInt16Callbacks.end(),
                                                       [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                        tcp::vUInt16Callbacks.end());
        }
        else if constexpr (std::is_same_v<T, int16_t>)
        {
            // Remove the callback function from the vector of int16_t callbacks
            tcp::vInt16Callbacks.erase(std::remove_if(tcp::vInt16Callbacks.begin(),
                                                      tcp::vInt16Callbacks.end(),
                                                      [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                       tcp::vInt16Callbacks.end());
        }
        else if constexpr (std::is_same_v<T, uint32_t>)
        {
            // Remove the callback function from the vector of uint32_t callbacks
            tcp::vUInt32Callbacks.erase(std::remove_if(tcp::vUInt32Callbacks.begin(),
                                                       tcp::vUInt32Callbacks.end(),
                                                       [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                        tcp::vUInt32Callbacks.end());
        }
        else if constexpr (std::is_same_v<T, int32_t>)
        {
            // Remove the callback function from the vector of int32_t callbacks
            tcp::vInt32Callbacks.erase(std::remove_if(tcp::vInt32Callbacks.begin(),
                                                      tcp::vInt32Callbacks.end(),
                                                      [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                       tcp::vInt32Callbacks.end());
        }
        else if constexpr (std::is_same_v<T, float>)
        {
            // Remove the callback function from the vector of float callbacks
            tcp::vFloatCallbacks.erase(std::remove_if(tcp::vFloatCallbacks.begin(),
                                                      tcp::vFloatCallbacks.end(),
                                                      [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                       tcp::vFloatCallbacks.end());
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            // Remove the callback function from the vector of double callbacks
            tcp::vDoubleCallbacks.erase(std::remove_if(tcp::vDoubleCallbacks.begin(),
                                                       tcp::vDoubleCallbacks.end(),
                                                       [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                        tcp::vDoubleCallbacks.end());
        }
        else if constexpr (std::is_same_v<T, char>)
        {
            // Remove the callback function from the vector of char callbacks
            tcp::vCharCallbacks.erase(std::remove_if(tcp::vCharCallbacks.begin(),
                                                     tcp::vCharCallbacks.end(),
                                                     [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                      tcp::vCharCallbacks.end());
        }
    }

    /******************************************************************************
     * @brief Processes a received packet and invokes the appropriate callback
     *        function from the vector of TCP callbacks for the specified data type.
     *        The data is unpacked into a RoveCommPacket and the appropriate
     *        callback function is invoked based on the data id.
     *
     * @tparam T - The data type of the RoveCommPacket. Must be one of the
     *             following: uint8_t, int8_t, uint16_t, int16_t, uint32_t,
     *             int32_t, float, double, or char.
     * @param stData - The received RoveCommData to process and invoke the
     *                 appropriate callback function.
     * @param vCallbacks - The vector of TCP callbacks for the specified data type.
     *                     The callback function will be invoked based on the data id
     *                     of the received packet.
     *
     * @note This method is not intended to be called directly. It is called by
     *       the ReceiveTCPPacketAndCallback method to process a received packet
     *       and invoke the appropriate callback function.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    template<typename T>
    void RoveCommTCP::ProcessPacket(const RoveCommData& stData,
                                    const std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<T>&)>, uint16_t>>& vCallbacks)
    {
        // Create instance variables.
        RoveCommPacket<T> stPacket = UnpackData<T>(stData);

        // Acquire a read lock to protect the callback vectors.
        std::shared_lock<std::shared_mutex> lkCallbackLock(m_muCallbackMutex);

        // Invoke registered callbacks
        for (const std::tuple<std::function<void(const rovecomm::RoveCommPacket<T>&)>, uint16_t>& tpCallbackInfo : vCallbacks)
        {
            const std::function<void(const rovecomm::RoveCommPacket<T>&)>& fnCallback = std::get<0>(tpCallbackInfo);
            const uint16_t& unCondition                                               = std::get<1>(tpCallbackInfo);

            if (unCondition == stPacket.unDataId)
            {
                fnCallback(stPacket);
            }
        }
    }

    /******************************************************************************
     * @brief Receives a TCP packet from a client and invokes the appropriate
     *        callback function from the vector of TCP callbacks for the specified
     *        data type. The data is unpacked into a RoveCommPacket and the
     *        appropriate callback function is invoked based on the data id.
     *
     * @note This method is not intended to be called directly. It is called by
     *       the ThreadedContinuousCode method to receive a TCP packet from a client
     *       and invoke the appropriate callback function. This method will run
     *       indefinitely and will block the thread until a client connection is
     *       accepted and data is received.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    void RoveCommTCP::ReceiveTCPPacketAndCallback()
    {
        if (m_nCurrentTCPClientSocket == -1)
        {
            // Accept a client connection
            struct sockaddr_in m_saClientAddr;
            socklen_t sklClientAddrLen = sizeof(m_saClientAddr);
            m_nCurrentTCPClientSocket  = accept(m_nTCPSocket, (struct sockaddr*) &m_saClientAddr, &sklClientAddrLen);
        }
        else
        {
            // Receive data from the client
            RoveCommData stData;
            ssize_t siBytesReceived = recv(m_nCurrentTCPClientSocket, &stData, sizeof(stData), MSG_DONTWAIT);

            // Process the received packet and invoke the appropriate callback
            if (siBytesReceived != -1)
            {
                // Extract the data id from the received data
                uint16_t unDataId = (static_cast<uint16_t>(stData.unBytes[1]) << 8) | static_cast<uint16_t>(stData.unBytes[2]);

                // Determine the data type from the received data
                manifest::DataTypes eDataType = manifest::Helpers::GetDataTypeFromId(unDataId);

                // Convert RoveCommData to appropriate RoveCommPacket based on data type
                switch (eDataType)
                {
                    case manifest::DataTypes::UINT8_T: ProcessPacket<uint8_t>(stData, tcp::vUInt8Callbacks); break;
                    case manifest::DataTypes::INT8_T: ProcessPacket<int8_t>(stData, tcp::vInt8Callbacks); break;
                    case manifest::DataTypes::UINT16_T: ProcessPacket<uint16_t>(stData, tcp::vUInt16Callbacks); break;
                    case manifest::DataTypes::INT16_T: ProcessPacket<int16_t>(stData, tcp::vInt16Callbacks); break;
                    case manifest::DataTypes::UINT32_T: ProcessPacket<uint32_t>(stData, tcp::vUInt32Callbacks); break;
                    case manifest::DataTypes::INT32_T: ProcessPacket<int32_t>(stData, tcp::vInt32Callbacks); break;
                    case manifest::DataTypes::FLOAT_T: ProcessPacket<float>(stData, tcp::vFloatCallbacks); break;
                    case manifest::DataTypes::DOUBLE_T: ProcessPacket<double>(stData, tcp::vDoubleCallbacks); break;
                    case manifest::DataTypes::CHAR: ProcessPacket<char>(stData, tcp::vCharCallbacks); break;
                }

                // Close the client socket
                close(m_nCurrentTCPClientSocket);
                m_nCurrentTCPClientSocket = -1;
            }
            // Still waiting for data or connection return without error.
            else if (siBytesReceived == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
            {
                return;
            }
        }
    }

    /******************************************************************************
     * @brief The threaded continuous code for the TCP class. This method calls
     *        the ReceiveTCPPacketAndCallback method to receive a TCP packet from a
     *        client and invoke the appropriate callback function.
     *
     * @note This method is not intended to be called directly. It is called by
     *       the Start method to start the threaded continuous code.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    void RoveCommTCP::ThreadedContinuousCode()
    {
        ReceiveTCPPacketAndCallback();
    }

    /******************************************************************************
     * @brief The pooled linear code for the TCP class. This method is not
     *        implemented and will do nothing.
     *
     * @note This method is not intended to be called directly. It is called by
     *       the Start method to start the pooled linear code.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    void RoveCommTCP::PooledLinearCode() {}

    /******************************************************************************
     * @brief Closes the TCP socket.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    void RoveCommTCP::CloseTCPSocket()
    {
        // Check if the TCP socket is open
        if (m_nTCPSocket != -1)
        {
            // Stop the threaded continuous code
            RequestStop();
            Join();

            // Close the TCP socket
            close(m_nTCPSocket);
        }
    }

    // Explicitly define template function types for TCP class
    template ssize_t RoveCommTCP::SendTCPPacket<uint8_t>(const RoveCommPacket<uint8_t>&, const char*, int);
    template void RoveCommTCP::AddTCPCallback<uint8_t>(std::function<void(const RoveCommPacket<uint8_t>&)>, const uint16_t&);
    template void RoveCommTCP::RemoveTCPCallback<uint8_t>(std::function<void(const RoveCommPacket<uint8_t>&)>);
    template void RoveCommTCP::ProcessPacket<uint8_t>(const RoveCommData& stData,
                                                      const std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<uint8_t>&)>, uint16_t>>& vCallbacks);

    template ssize_t RoveCommTCP::SendTCPPacket<int8_t>(const RoveCommPacket<int8_t>&, const char*, int);
    template void RoveCommTCP::AddTCPCallback<int8_t>(std::function<void(const RoveCommPacket<int8_t>&)>, const uint16_t&);
    template void RoveCommTCP::RemoveTCPCallback<int8_t>(std::function<void(const RoveCommPacket<int8_t>&)>);

    template ssize_t RoveCommTCP::SendTCPPacket<uint16_t>(const RoveCommPacket<uint16_t>&, const char*, int);
    template void RoveCommTCP::AddTCPCallback<uint16_t>(std::function<void(const RoveCommPacket<uint16_t>&)>, const uint16_t&);
    template void RoveCommTCP::RemoveTCPCallback<uint16_t>(std::function<void(const RoveCommPacket<uint16_t>&)>);

    template ssize_t RoveCommTCP::SendTCPPacket<int16_t>(const RoveCommPacket<int16_t>&, const char*, int);
    template void RoveCommTCP::AddTCPCallback<int16_t>(std::function<void(const RoveCommPacket<int16_t>&)>, const uint16_t&);
    template void RoveCommTCP::RemoveTCPCallback<int16_t>(std::function<void(const RoveCommPacket<int16_t>&)>);

    template ssize_t RoveCommTCP::SendTCPPacket<uint32_t>(const RoveCommPacket<uint32_t>&, const char*, int);
    template void RoveCommTCP::AddTCPCallback<uint32_t>(std::function<void(const RoveCommPacket<uint32_t>&)>, const uint16_t&);
    template void RoveCommTCP::RemoveTCPCallback<uint32_t>(std::function<void(const RoveCommPacket<uint32_t>&)>);

    template ssize_t RoveCommTCP::SendTCPPacket<int32_t>(const RoveCommPacket<int32_t>&, const char*, int);
    template void RoveCommTCP::AddTCPCallback<int32_t>(std::function<void(const RoveCommPacket<int32_t>&)>, const uint16_t&);
    template void RoveCommTCP::RemoveTCPCallback<int32_t>(std::function<void(const RoveCommPacket<int32_t>&)>);

    template ssize_t RoveCommTCP::SendTCPPacket<float>(const RoveCommPacket<float>&, const char*, int);
    template void RoveCommTCP::AddTCPCallback<float>(std::function<void(const RoveCommPacket<float>&)>, const uint16_t&);
    template void RoveCommTCP::RemoveTCPCallback<float>(std::function<void(const RoveCommPacket<float>&)>);

    template ssize_t RoveCommTCP::SendTCPPacket<double>(const RoveCommPacket<double>&, const char*, int);
    template void RoveCommTCP::AddTCPCallback<double>(std::function<void(const RoveCommPacket<double>&)>, const uint16_t&);
    template void RoveCommTCP::RemoveTCPCallback<double>(std::function<void(const RoveCommPacket<double>&)>);

    template ssize_t RoveCommTCP::SendTCPPacket<char>(const RoveCommPacket<char>&, const char*, int);
    template void RoveCommTCP::AddTCPCallback<char>(std::function<void(const RoveCommPacket<char>&)>, const uint16_t&);
    template void RoveCommTCP::RemoveTCPCallback<char>(std::function<void(const RoveCommPacket<char>&)>);
}    // namespace rovecomm
