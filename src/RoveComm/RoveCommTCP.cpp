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

#if defined(__ROVECOMM_WINDOWS_MODE__) && __ROVECOMM_WINDOWS_MODE__ == 1
typedef SOCKET socket_t;
#define CLOSE_SOCKET   closesocket
#define GET_LAST_ERROR WSAGetLastError()
#else
typedef int socket_t;
#define CLOSE_SOCKET   close
#define GET_LAST_ERROR errno
#endif

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
        Close();
    }

    /******************************************************************************
     * @brief Initializes a TCP socket and binds it to the specified IP address and
     *        port. And then starts the threaded continuous code in AutonomyThread.
     *
     * @param stIPAddress - The IP address to bind the socket to. If set to "", the
     *                     socket will be bound to all available interfaces.
     * @param nPort - The port to bind the socket to. If set to 0, the OS will
     *                automatically assign an available port.
     * @return true - The TCP socket was successfully initialized and bound.
     * @return false - The TCP socket failed to initialize and bind.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    bool RoveCommTCP::Init(const manifest::AddressEntry& stIPAddress, int nPort)
    {
#if defined(__ROVECOMM_WINDOWS_MODE__) && __ROVECOMM_WINDOWS_MODE__ == 1
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            std::cerr << "WSAStartup failed." << std::endl;
            return 1;
        }
#endif

        // Create a TCP socket
        m_nTCPSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_nTCPSocket == -1)
        {
            perror("Failed to create TCP socket");
            return false;
        }
        else
        {
#if defined(__ROVECOMM_WINDOWS_MODE__) && __ROVECOMM_WINDOWS_MODE__ == 1
            u_long mode = 1;    // 1 to enable non-blocking mode
            if (ioctlsocket(m_nTCPSocket, FIONBIO, &mode) == SOCKET_ERROR)
            {
                // Handle and print error.
                int err = WSAGetLastError();
                fprintf(stderr, "Failed to set UDP socket to non-blocking mode. Error code: %d\n", err);
                return false;
            }
#else
            if (fcntl(m_nTCPSocket, F_SETFL, fcntl(m_nTCPSocket, F_GETFL) | O_NONBLOCK) == -1)
            {
                // Handle and print error.
                perror("Failed to set UDP socket to non-blocking mode.");
                return false;
            }
#endif
        }

        // Configure the server address
        memset(&m_saTCPServerAddr, 0, sizeof(m_saTCPServerAddr));
        m_saTCPServerAddr.sin_family = AF_INET;
        m_saTCPServerAddr.sin_addr.s_addr =
            htonl(stIPAddress.FIRST_OCTET << 24 | stIPAddress.SECOND_OCTET << 16 | stIPAddress.THIRD_OCTET << 8 | stIPAddress.FOURTH_OCTET);
        m_saTCPServerAddr.sin_port = htons(nPort);

        // Bind the socket to the server address
        if (bind(m_nTCPSocket.load(), (struct sockaddr*) &m_saTCPServerAddr, sizeof(m_saTCPServerAddr)) == -1)
        {
            perror("Failed to bind TCP socket");
            CLOSE_SOCKET(m_nTCPSocket);
            return false;
        }

        // Listen for incoming connections
        if (listen(m_nTCPSocket, 5) == -1)
        {
            perror("Failed to listen on TCP socket");
            CLOSE_SOCKET(m_nTCPSocket);
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
     * @param stClientIPAddress - The IP address of the client to send the packet to.
     * @param nClientPort - The port of the client to send the packet to.
     * @return ssize_t - The number of bytes sent over the TCP socket. Returns -1
     *                   if an error occurred.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    template<typename T>
    ssize_t RoveCommTCP::Send(const RoveCommPacket<T>& stPacket, const manifest::AddressEntry& stClientIPAddress, int nClientPort)
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
        saClientAddr.sin_addr.s_addr =
            htonl(stClientIPAddress.FIRST_OCTET << 24 | stClientIPAddress.SECOND_OCTET << 16 | stClientIPAddress.THIRD_OCTET << 8 | stClientIPAddress.FOURTH_OCTET);
        // Connect to the client
        if (connect(nClientSocket, (struct sockaddr*) &saClientAddr, sizeof(saClientAddr)) == -1)
        {
            perror("Connection failed");
            CLOSE_SOCKET(nClientSocket);
            return -1;
        }

        // Pack the data
        std::vector<uint8_t> vData = PackPacket(stPacket);
        // Get size of data not including the data not filled. (the null/zero data in RoveCommData)
        size_t siDataSize = ROVECOMM_PACKET_HEADER_SIZE + stPacket.GetDataSize();

        // Acquire a write lock on the socket send mutex to protect the socket, which is shared between threads, but not thread-safe.
        std::unique_lock<std::mutex> lkSocketSendLock(m_muSocketSendMutex);
        // Send the data
#if defined(__ROVECOMM_WINDOWS_MODE__) && __ROVECOMM_WINDOWS_MODE__ == 1
        ssize_t siBytesSent = send(nClientSocket, reinterpret_cast<char*>(&stData), siDataSize, 0);
#else
        ssize_t siBytesSent = send(nClientSocket, vData.data(), siDataSize, 0);
#endif
        // Check if any bytes were sent.
        if (siBytesSent == -1)
        {
            // Handle and print error message.
            perror("Failed to send data to TCP client socket");
        }

        // Close the client socket
        CLOSE_SOCKET(nClientSocket);

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
    void RoveCommTCP::On(const uint16_t unDataId, std::function<void(const RoveCommPacket<T>&)> fnCallback)
    {
        // Acquire a write lock to protect the callback vectors.
        std::unique_lock<std::shared_mutex> lkCallbackLock(m_muCallbackMutex);

        // Add the callback function to the vector of TCP callbacks for the specified data type
        // Note: C++ will default initialize a new vector if no entry is found.
        RoveCommRegistry<T>::umCallbackMap[unDataId].push_back(fnCallback);
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
    void RoveCommTCP::Clear(const uint16_t unDataId)
    {
        // Acquire a write lock to protect the callback vectors.
        std::unique_lock<std::shared_mutex> lkCallbackLock(m_muCallbackMutex);

        // Remove the callback function from the vector of UDP callbacks for the specified data type
        std::erase_if(RoveCommRegistry<T>::umCallbackMap, [unDataId](const auto& stdEntry) { return stdEntry.first == unDataId; });
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
    void RoveCommTCP::ProcessPacket(std::span<const uint8_t> spData)
    {
        // Create instance variables.
        RoveCommPacket<T> stPacket = UnpackData<T>(spData);

        // Acquire a read lock to protect the callback vectors.
        std::shared_lock<std::shared_mutex> lkCallbackLock(m_muCallbackMutex);

        // Invoke registered callbacks
        for (const auto& [unDataId, vCallbacks] : RoveCommRegistry<T>::umCallbackMap)
        {
            if (unDataId == stPacket.unDataId)
            {
                for (const auto& fnCallback : vCallbacks)
                {
                    fnCallback(stPacket);
                }
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
    void RoveCommTCP::ReceiveAndCallback()
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
            std::array<uint8_t, ROVECOMM_PACKET_MAX_DATA_SIZE> aData;
#if defined(__ROVECOMM_WINDOWS_MODE__) && __ROVECOMM_WINDOWS_MODE__ == 1
            ssize_t siBytesReceived = recv(m_nCurrentTCPClientSocket, reinterpret_cast<char*>(aData.data()), sizeof(aData), 0);
#else
            ssize_t siBytesReceived = recv(m_nCurrentTCPClientSocket, aData.data(), sizeof(aData), MSG_DONTWAIT);
#endif

            if (siBytesReceived < 0)
            {
                // Still waiting for data or connection return without error.
                if (errno != EAGAIN && errno != EWOULDBLOCK)
                {
                    perror("Failed to receive data from TCP socket using recv.");
                }
                return;
            }
            // Process the received packet and invoke the appropriate callback

            if (siBytesReceived < ROVECOMM_PACKET_HEADER_SIZE)
            {
                throw std::runtime_error("Not enough data to parse RoveCommPacket header.");
            }
            // Extract the data id from the received data
            uint16_t unDataId = (static_cast<uint16_t>(aData[1]) << 8) | static_cast<uint16_t>(aData[2]);

            // Determine the data type from the received data
            manifest::DataTypes eDataType = static_cast<manifest::DataTypes>(aData[5]);

            // Convert RoveCommData to appropriate RoveCommPacket based on data type
            switch (eDataType)
            {
                case manifest::DataTypes::UINT8_T: ProcessPacket<uint8_t>(aData); break;
                case manifest::DataTypes::INT8_T: ProcessPacket<int8_t>(aData); break;
                case manifest::DataTypes::UINT16_T: ProcessPacket<uint16_t>(aData); break;
                case manifest::DataTypes::INT16_T: ProcessPacket<int16_t>(aData); break;
                case manifest::DataTypes::UINT32_T: ProcessPacket<uint32_t>(aData); break;
                case manifest::DataTypes::INT32_T: ProcessPacket<int32_t>(aData); break;
                case manifest::DataTypes::FLOAT_T: ProcessPacket<float>(aData); break;
                case manifest::DataTypes::DOUBLE_T: ProcessPacket<double>(aData); break;
                case manifest::DataTypes::CHAR: ProcessPacket<char>(aData); break;
            }

            // Close the client socket
            CLOSE_SOCKET(m_nCurrentTCPClientSocket);
            m_nCurrentTCPClientSocket = -1;
            // TODO: Keep connections open after recv()
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
        ReceiveAndCallback();
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
    void RoveCommTCP::Close()
    {
        // Check if the TCP socket is open
        if (m_nTCPSocket != -1)
        {
            // Stop the threaded continuous code
            RequestStop();
            Join();

            // Close the TCP socket
            CLOSE_SOCKET(m_nTCPSocket);

#if defined(__ROVECOMM_WINDOWS_MODE__) && __ROVECOMM_WINDOWS_MODE__ == 1
            WSACleanup();
#endif
        }
    }

    // Explicitly define template function types for TCP class
    template ssize_t RoveCommTCP::Send<uint8_t>(const RoveCommPacket<uint8_t>&, const manifest::AddressEntry&, int);
    template void RoveCommTCP::On<uint8_t>(const uint16_t, std::function<void(const RoveCommPacket<uint8_t>&)>);
    template void RoveCommTCP::Clear<uint8_t>(const uint16_t);

    template ssize_t RoveCommTCP::Send<int8_t>(const RoveCommPacket<int8_t>&, const manifest::AddressEntry&, int);
    template void RoveCommTCP::On<int8_t>(const uint16_t, std::function<void(const RoveCommPacket<int8_t>&)>);
    template void RoveCommTCP::Clear<int8_t>(const uint16_t);

    template ssize_t RoveCommTCP::Send<uint16_t>(const RoveCommPacket<uint16_t>&, const manifest::AddressEntry&, int);
    template void RoveCommTCP::On<uint16_t>(const uint16_t, std::function<void(const RoveCommPacket<uint16_t>&)>);
    template void RoveCommTCP::Clear<uint16_t>(const uint16_t);

    template ssize_t RoveCommTCP::Send<int16_t>(const RoveCommPacket<int16_t>&, const manifest::AddressEntry&, int);
    template void RoveCommTCP::On<int16_t>(const uint16_t, std::function<void(const RoveCommPacket<int16_t>&)>);
    template void RoveCommTCP::Clear<int16_t>(const uint16_t);

    template ssize_t RoveCommTCP::Send<uint32_t>(const RoveCommPacket<uint32_t>&, const manifest::AddressEntry&, int);
    template void RoveCommTCP::On<uint32_t>(const uint16_t, std::function<void(const RoveCommPacket<uint32_t>&)>);
    template void RoveCommTCP::Clear<uint32_t>(const uint16_t);

    template ssize_t RoveCommTCP::Send<int32_t>(const RoveCommPacket<int32_t>&, const manifest::AddressEntry&, int);
    template void RoveCommTCP::On<int32_t>(const uint16_t, std::function<void(const RoveCommPacket<int32_t>&)>);
    template void RoveCommTCP::Clear<int32_t>(const uint16_t);

    template ssize_t RoveCommTCP::Send<float>(const RoveCommPacket<float>&, const manifest::AddressEntry&, int);
    template void RoveCommTCP::On<float>(const uint16_t, std::function<void(const RoveCommPacket<float>&)>);
    template void RoveCommTCP::Clear<float>(const uint16_t);

    template ssize_t RoveCommTCP::Send<double>(const RoveCommPacket<double>&, const manifest::AddressEntry&, int);
    template void RoveCommTCP::On<double>(const uint16_t, std::function<void(const RoveCommPacket<double>&)>);
    template void RoveCommTCP::Clear<double>(const uint16_t);

    template ssize_t RoveCommTCP::Send<char>(const RoveCommPacket<char>&, const manifest::AddressEntry&, int);
    template void RoveCommTCP::On<char>(const uint16_t, std::function<void(const RoveCommPacket<char>&)>);
    template void RoveCommTCP::Clear<char>(const uint16_t);
}    // namespace rovecomm
