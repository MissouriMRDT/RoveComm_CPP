/******************************************************************************
 * @brief The RoveCommUDP class is used to send and receive data over a UDP
 *        connection.
 *
 * @file RoveCommUDP.cpp
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-07
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#include "RoveCommUDP.h"
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
#if defined(__ROVECOMM_WINDOWS_MODE__) && __ROVECOMM_WINDOWS_MODE__ == 1
    /******************************************************************************
     * @brief This struct is used to store the context for an asynchronous receive
     *      operation. This is used to receive multiple packets at once.
     *
     *
     * @author clayjay3 (claytonraycowen@gmail.com)
     * @date 2025-02-18
     ******************************************************************************/
    struct RecvContext
    {
            OVERLAPPED overlapped;
            std::array<uint8_t, ROVECOMM_PACKET_MAX_DATA_SIZE> data;
            sockaddr_in addr;
            WSABUF wsabuf;
    };

    /******************************************************************************
     * @brief This struct is used to store the context for an asynchronous send
     *      operation. This is used to send multiple packets at once.
     *
     *
     * @author clayjay3 (claytonraycowen@gmail.com)
     * @date 2025-02-18
     ******************************************************************************/
    struct SendContext
    {
            OVERLAPPED overlapped;
            WSABUF wsabuf;
            sockaddr_in addr;
    };
#endif

    /******************************************************************************
     * @brief Construct a new RoveCommUDP::RoveCommUDP object.
     *
     *
     * @author clayjay3 (claytonraycowen@gmail.com)
     * @date 2024-03-07
     ******************************************************************************/
    RoveCommUDP::RoveCommUDP()
    {
        // Initialize member variables.
        m_nUDPSocket = -1;

        // Set an IPS cap in the backend RoveComm thread.
        this->SetMainThreadIPSLimit(rovecomm::ROVECOMM_THREAD_MAX_IPS);
    }

    /******************************************************************************
     * @brief Destroy the RoveCommUDP::RoveCommUDP object.
     *
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    RoveCommUDP::~RoveCommUDP()
    {
        Close();
    }

    /******************************************************************************
     * @brief Initialize the UDP socket and bind it to the specified port. This
     *        method also starts the thread that will continuously receive UDP
     *        packets and invoke the appropriate callback function.
     *
     * @param nPort - The port that the UDP socket is to be bound to. If set to 0,
     *                then the operating system will automatically assign to an
     *                available port.
     * @return true - The UDP socket was successfully initialized and bound.
     * @return false - An error occurred while initializing the UDP socket.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    bool RoveCommUDP::Init(int nPort)
    {
#if defined(__ROVECOMM_WINDOWS_MODE__) && __ROVECOMM_WINDOWS_MODE__ == 1
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            std::cerr << "WSAStartup failed." << std::endl;
            return 1;
        }
#endif

        // Create a UDP socket
        m_nUDPSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (m_nUDPSocket == -1)
        {
            perror("Failed to create UDP socket");
            return false;
        }
        else
        {
            // Increase the socket buffer size to 1MB
            int bufferSize = 1 * 1024 * 1024;
            if (setsockopt(m_nUDPSocket, SOL_SOCKET, SO_RCVBUF, (char*) &bufferSize, sizeof(bufferSize)) == -1)
            {
                perror("Failed to set socket receive buffer size");
                return false;
            }
            if (setsockopt(m_nUDPSocket, SOL_SOCKET, SO_SNDBUF, (char*) &bufferSize, sizeof(bufferSize)) == -1)
            {
                perror("Failed to set socket send buffer size");
                return false;
            }

            // Set SO_REUSEADDR and SO_REUSEPORT
            int optval = 1;
            if (setsockopt(m_nUDPSocket, SOL_SOCKET, SO_REUSEADDR, (char*) &optval, sizeof(optval)) == -1)
            {
                perror("Failed to set SO_REUSEADDR");
                return false;
            }
#ifdef SO_REUSEPORT
            if (setsockopt(m_nUDPSocket, SOL_SOCKET, SO_REUSEPORT, (char*) &optval, sizeof(optval)) == -1)
            {
                perror("Failed to set SO_REUSEPORT");
                return false;
            }
#endif

#if defined(__ROVECOMM_WINDOWS_MODE__) && __ROVECOMM_WINDOWS_MODE__ == 1
            // For asynchronous I/O, we use overlapped operations instead of non-blocking mode.
            // Create an IOCP and associate our UDP socket with it.
            m_stdIOCP = CreateIoCompletionPort((HANDLE) (SOCKET) m_nUDPSocket.load(), NULL, 0, 0);
            if (m_stdIOCP == NULL)
            {
                perror("CreateIoCompletionPort failed.");
                return false;
            }
#else
            if (fcntl(m_nUDPSocket, F_SETFL, fcntl(m_nUDPSocket, F_GETFL) | O_NONBLOCK) == -1)
            {
                // Handle and print error.
                perror("Failed to set UDP socket to non-blocking mode.");
                return false;
            }
#endif
        }

        // Configure the server address
        sockaddr_in saServerAddr;
        memset(&saServerAddr, 0, sizeof(saServerAddr));
        saServerAddr.sin_family      = AF_INET;
        saServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        saServerAddr.sin_port        = htons(nPort);

        // Bind the socket
        if (bind(m_nUDPSocket.load(), (struct sockaddr*) &saServerAddr, sizeof(saServerAddr)) == -1)
        {
            perror("Failed to bind UDP socket");
            CLOSE_SOCKET(m_nUDPSocket);
            return false;
        }

        // Start the thread
        Start();

        return true;
    }

    /******************************************************************************
     * @brief Send a UDP packet to the specified IP address and port. Converts the
     *        RoveCommPacket into bytes (RoveCommData) and sends it to the specified
     *        IP address and port.
     *
     * @tparam T - The type of data that is to be sent. This can be any of the types
     *             defined in the manifest.
     * @param stPacket - The RoveCommPacket that is to be sent.
     * @param cIPAddress - The IP address of the client that the packet is to be sent to.
     * @param nPort - The port that the packet is to be sent to.
     * @return ssize_t - The number of bytes that were sent. If the return value is
     *                   less than 0, then an error occurred.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    template<typename T>
    ssize_t RoveCommUDP::Send(const RoveCommPacket<T>& stPacket, const manifest::AddressEntry& stIPAddress, int nPort)
    {
        // Pack the RoveCommPacket into a RoveCommData structure
        std::vector<uint8_t> vData = PackPacket(stPacket);
        // Get size of data not including the data not filled. (the null/zero data in RoveCommData)
        size_t siDataSize = ROVECOMM_PACKET_HEADER_SIZE + stPacket.GetDataSize();

#if defined(__ROVECOMM_WINDOWS_MODE__) && __ROVECOMM_WINDOWS_MODE__ == 1
        // Setup the base UDP client address
        struct sockaddr_in saUDPClientAddr;
        memset(&saUDPClientAddr, 0, sizeof(saUDPClientAddr));
        saUDPClientAddr.sin_family = AF_INET;

        // Send the packet to all subscribers
        for (const auto& [stSubscriberIPAddress, nSubscriberPort] : vSubscribers)
        {
            // Assemble client address.
            saUDPClientAddr.sin_port        = htons(nSubscriberPort);
            saUDPClientAddr.sin_addr.s_addr = htonl(stSubscriberIPAddress.FIRST_OCTET << 24 | stSubscriberIPAddress.SECOND_OCTET << 16 |
                                                    stSubscriberIPAddress.THIRD_OCTET << 8 | stSubscriberIPAddress.FOURTH_OCTET);
            // Acquire a write lock on the socket send mutex to protect the socket, which is shared between threads, but not thread-safe.
            std::unique_lock<std::mutex> lkSocketSendLock(m_muSocketSendMutex);
            // Send data.
            if (sendto(m_nUDPSocket, reinterpret_cast<char*>(&spData), siDataSize, 0, (struct sockaddr*) &saUDPClientAddr, sizeof(saUDPClientAddr)) == -1)
            {
                // Handle and print error message.
                perror("Failed to send data to UDP client socket subscriber.");
            }
            // Release the lock.
            lkSocketSendLock.unlock();
        }

        // Send the packet to the specified IP address and port.
        if (stIPAddress != manifest::AddressEntry{0, 0, 0, 0} && nPort != 0)
        {
            saUDPClientAddr.sin_port = htons(nPort);
            saUDPClientAddr.sin_addr.s_addr =
                htonl(stIPAddress.FIRST_OCTET << 24 | stIPAddress.SECOND_OCTET << 16 | stIPAddress.THIRD_OCTET << 8 | stIPAddress.FOURTH_OCTET);

            // Acquire a write lock on the socket send mutex to protect the socket, which is shared between threads, but not thread-safe.
            std::unique_lock<std::mutex> lkSocketSendLock(m_muSocketSendMutex);
            return sendto(m_nUDPSocket, reinterpret_cast<char*>(vData.data()), siDataSize, 0, (struct sockaddr*) &saUDPClientAddr, sizeof(saUDPClientAddr));
        }

        return -1;
#else
        // Use sendmmsg to send the same packet to multiple destinations in one call.
        std::vector<sockaddr_in> vDestinations;
        // Add all subscribers.
        for (const auto& [stSubscriberIPAddress, nSubscriberPort] : seSubscribers)
        {
            // Assemble client address.
            sockaddr_in stdAddr{};
            stdAddr.sin_family = AF_INET;
            stdAddr.sin_port   = htons(nSubscriberPort);
            // Convert the IP address to binary form.
            stdAddr.sin_addr.s_addr = htonl(stSubscriberIPAddress.FIRST_OCTET << 24 | stSubscriberIPAddress.SECOND_OCTET << 16 | stSubscriberIPAddress.THIRD_OCTET << 8 |
                                            stSubscriberIPAddress.FOURTH_OCTET);
            // Add the subscriber to the list of destinations.
            vDestinations.push_back(stdAddr);
        }
        // Add the specified destination if provided.
        if (stIPAddress != manifest::AddressEntry{0, 0, 0, 0} && nPort != 0)
        {
            // Assemble client address.
            sockaddr_in stdAddr{};
            stdAddr.sin_family = AF_INET;
            stdAddr.sin_port   = htons(nPort);
            // Convert the IP address to binary form.
            stdAddr.sin_addr.s_addr = htonl(stIPAddress.FIRST_OCTET << 24 | stIPAddress.SECOND_OCTET << 16 | stIPAddress.THIRD_OCTET << 8 | stIPAddress.FOURTH_OCTET);
            // Add the specified destination to the list of destinations.
            vDestinations.push_back(stdAddr);
        }

        // Send the packet to all destinations.
        if (vDestinations.empty())
        {
            return -1;
        }

        // Get the number of destinations and create the necessary data structures.
        size_t siCount = vDestinations.size();
        std::vector<struct mmsghdr> vMsgVec(siCount);
        std::vector<struct iovec> vIOVecs(siCount);
        // Send the packet to all destinations.
        for (size_t siIter = 0; siIter < siCount; siIter++)
        {
            // Set the data to be sent.
            vIOVecs[siIter].iov_base = reinterpret_cast<char*>(vData.data());
            vIOVecs[siIter].iov_len  = siDataSize;
            memset(&vMsgVec[siIter], 0, sizeof(struct mmsghdr));
            // Set the destination address.
            vMsgVec[siIter].msg_hdr.msg_iov    = &vIOVecs[siIter];
            vMsgVec[siIter].msg_hdr.msg_iovlen = 1;
            vMsgVec[siIter].msg_hdr.msg_name   = &vDestinations[siIter];
            // Set the length of the destination address.
            vMsgVec[siIter].msg_hdr.msg_namelen = sizeof(sockaddr_in);
        }

        // Acquire a write lock on the socket send mutex to protect the socket, which is shared between threads, but not thread-safe.
        std::unique_lock<std::mutex> lkSocketSendLock(m_muSocketSendMutex);
        // Send the packet to all destinations.
        int nSentMessages = sendmmsg(m_nUDPSocket, vMsgVec.data(), siCount, 0);
        if (nSentMessages == -1)
        {
            perror("sendmmsg error");
            return -1;
        }
        // Do the math to get the number of bytes sent.
        ssize_t siBytesSent = 0;
        for (size_t siIter = 0; siIter < siCount; siIter++)
        {
            siBytesSent += vIOVecs[siIter].iov_len;
        }

        // Return the number of packets sent.
        return siBytesSent;
#endif
    }

    /******************************************************************************
     * @brief Add a callback function to the list of UDP callbacks. The callback
     *        function will be invoked when a packet with the specified data id is
     *        received.
     *
     * @tparam T - The type of data that the callback function will be invoked with.
     *             This can be any of the types defined in the manifest.
     * @param fnCallback - The callback function that is to be added to the list of
     *                     UDP callbacks.
     * @param unCondition - The data id that the callback function is to be invoked
     *                      with. The callback function will only be invoked when a
     *                      packet with this data id is received.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    template<typename T>
    void RoveCommUDP::On(const uint16_t unDataId, std::function<void(const RoveCommPacket<T>&)> fnCallback)
    {
        // Acquire a write lock to protect the callback vectors.
        std::unique_lock<std::shared_mutex> lkCallbackLock(m_muCallbackMutex);

        // Add the callback function to the vector of UDP callbacks for the specified data type
        // Note: C++ will default initialize a new vector if no entry is found.
        RoveCommRegistry<T>::umCallbackMap[unDataId].push_back(fnCallback);
    }

    // template<typename T>
    // void On(const std::string& szBoardName, const std::string& szPacketName, std::function<void(const RoveCommPacket<T>&)> fnCallback)
    // {
    //     On(unDataId, fnCallback);
    // }

    /******************************************************************************
     * @brief Remove a callback function from the list of UDP callbacks. The callback
     *        function will no longer be invoked when a packet with the specified
     *        data id is received.
     *
     * @tparam T - The type of data that the callback function will be invoked with.
     *             This can be any of the types defined in the manifest.
     * @param fnCallback - The callback function that is to be removed from the list
     *                     of UDP callbacks.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    template<typename T>
    void RoveCommUDP::Clear(const uint16_t unDataId)
    {
        // Acquire a write lock to protect the callback vectors.
        std::unique_lock<std::shared_mutex> lkCallbackLock(m_muCallbackMutex);

        // Remove the callback function from the vector of UDP callbacks for the specified data type
        std::erase_if(RoveCommRegistry<T>::umCallbackMap, [unDataId](const auto& stdEntry) { return stdEntry.first == unDataId; });
    }

    /******************************************************************************
     * @brief Process a UDP packet and invoke the appropriate callback function.
     *        This function is called from the ReceiveUDPPacketAndCallback function
     *        and is used to invoke the appropriate callback function based on the
     *        data id of the received packet. The callback function is invoked with
     *        the received packet and the client address.
     *
     * @tparam T - The type of data that the callback function will be invoked with.
     *             This can be any of the types defined in the manifest.
     * @param spData - The received RoveCommData that is to be processed.
     * @param vCallbacks - The list of callback functions that are to be invoked when
     *                    a packet with the specified data id is received.
     * @param saClientAddr - The address of the client that sent the packet.
     *
     * @note This function is not intended to be called directly. It is called from
     *       the ReceiveUDPPacketAndCallback function.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    template<typename T>
    void RoveCommUDP::ProcessPacket(std::span<const uint8_t> spData, const sockaddr_in& saClientAddr)
    {
        // Unpack the received data into a RoveCommPacket
        RoveCommPacket<T> stPacket = UnpackData<T>(spData);

        // Create a SubscriberInfo struct to store the client address
        std::string t = inet_ntoa(saClientAddr.sin_addr);
        int nPort     = ntohs(saClientAddr.sin_port);

        // Check if the received packet is a subscribe or unsubscribe packet
        if (stPacket.unDataId == manifest::System::SUBSCRIBE_DATA_ID)
        {
            AddSubscriber(t, nPort);
        }
        else if (stPacket.unDataId == manifest::System::UNSUBSCRIBE_DATA_ID)
        {
            RemoveSubscriber(t, nPort);
        }

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
     * @brief Receive a UDP packet and invoke the appropriate callback function.
     *        Since data types are not known at compile time, this function calls
     *        the appropriate ProcessPacket function based on the data type of the
     *        received packet.
     *
     * @note This function is not intended to be called directly. It is called from
     *       the ThreadedContinuousCode function. If this is called directly, it
     *       will block the thread until a packet is received.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    void RoveCommUDP::ReceiveAndCallback()
    {
#if defined(__ROVECOMM_WINDOWS_MODE__) && __ROVECOMM_WINDOWS_MODE__ == 1
        // Create a batch of RecvContext structures to receive multiple packets at once.
        constexpr size_t BATCH_SIZE = 32;
        RecvContext rcContexts[BATCH_SIZE];

        // Post BATCH_SIZE asynchronous receives.
        for (size_t siIter = 0; siIter < BATCH_SIZE; siIter++)
        {
            ZeroMemory(&rcContexts[siIter].overlapped, sizeof(OVERLAPPED));
            rcContexts[siIter].wsabuf.buf = rcContexts[siIter].data.data();
            rcContexts[siIter].wsabuf.len = rcContexts[siIter].data.size();
            int nAddrLen                  = sizeof(rcContexts[siIter].addr);
            DWORD stdFlags                = 0;
            int nRet                      = WSARecvFrom(m_nUDPSocket,
                                                        &rcContexts[siIter].wsabuf,
                                                        1,
                                                        NULL,
                                                        &stdFlags,
                                                        reinterpret_cast<sockaddr*>(&rcContexts[siIter].addr),
                                                        &nAddrLen,
                                                        &rcContexts[siIter].overlapped,
                                                        NULL);
            if (nRet == SOCKET_ERROR)
            {
                int nErr = WSAGetLastError();
                if (nErr != WSA_IO_PENDING)
                {
                    perror("WSARecvFrom failed!");
                }
            }
        }

        // Wait for completions in a loop.
        DWORD stdNumberOfBytesTransferred;
        ULONG_PTR ulCompletionKey;
        LPOVERLAPPED stdLPOverlapped;
        // Set a short timeout (in milliseconds) to poll for completions.
        const DWORD dwTimeout = 1;    // 1ms timeout (adjust as needed).
        while (true)
        {
            // Wait for an I/O completion event with a short timeout.
            BOOL bSuccess = GetQueuedCompletionStatus(m_stdIOCP, &stdNumberOfBytesTransferred, &ulCompletionKey, &stdLPOverlapped, dwTimeout);
            if (!bSuccess)
            {
                if (GetLastError() == WAIT_TIMEOUT)
                {
                    // No completion available within the timeout period.
                    break;    // Exit the loop so the function returns.
                }
                else
                {
                    perror("GetQueuedCompletionStatus (recv) failed!");
                    continue;
                }
            }

            // If stdLPOverlapped is NULL, something went wrong.
            if (stdLPOverlapped == nullptr)
            {
                continue;
            }

            // Identify the RecvContext from the overlapped pointer.
            RecvContext* pContext = CONTAINING_RECORD(stdLPOverlapped, RecvContext, overlapped);

            // Process the received data.
            std::span<uint8_t> spData{pContext->data.data(), stdNumberOfBytesTransferred};
            sockaddr_in& saClientAddr = pContext->addr;

            // Extract data id and data type from the packet.
            uint16_t unDataId             = (static_cast<uint16_t>(spData[1]) << 8) | static_cast<uint16_t>(spData[2]);
            manifest::DataTypes eDataType = static_cast<manifest::DataTypes>(spData[5]);

            switch (eDataType)
            {
                case manifest::DataTypes::UINT8_T: ProcessPacket<uint8_t>(spData, saClientAddr); break;
                case manifest::DataTypes::INT8_T: ProcessPacket<int8_t>(spData, saClientAddr); break;
                case manifest::DataTypes::UINT16_T: ProcessPacket<uint16_t>(spData, saClientAddr); break;
                case manifest::DataTypes::INT16_T: ProcessPacket<int16_t>(spData, saClientAddr); break;
                case manifest::DataTypes::UINT32_T: ProcessPacket<uint32_t>(spData, saClientAddr); break;
                case manifest::DataTypes::INT32_T: ProcessPacket<int32_t>(spData, saClientAddr); break;
                case manifest::DataTypes::FLOAT_T: ProcessPacket<float>(spData, saClientAddr); break;
                case manifest::DataTypes::DOUBLE_T: ProcessPacket<double>(spData, saClientAddr); break;
                case manifest::DataTypes::CHAR: ProcessPacket<char>(spData, saClientAddr); break;
            }

            // Re-post the asynchronous receive for this context.
            ZeroMemory(&pContext->overlapped, sizeof(OVERLAPPED));
            pContext->wsabuf.buf = reinterpret_cast<char*>(pContext->data.data());
            pContext->wsabuf.len = sizeof(pContext->data);
            int nAddrLen         = sizeof(pContext->addr);
            DWORD stdFlags       = 0;
            int nRet =
                WSARecvFrom(m_nUDPSocket, &pContext->wsabuf, 1, NULL, &stdFlags, reinterpret_cast<sockaddr*>(&pContext->addr), &nAddrLen, &pContext->overlapped, NULL);
            if (nRet == SOCKET_ERROR)
            {
                int nErr = WSAGetLastError();
                if (nErr != WSA_IO_PENDING)
                {
                    perror("WSARecvFrom (re-post) failed!");
                }
            }
        }
#else
        // Create a batch of RoveCommData structures to receive multiple packets at once.
        constexpr size_t BATCH_SIZE = 32;
        std::array<uint8_t, ROVECOMM_PACKET_MAX_DATA_SIZE> aDataBatch[BATCH_SIZE];
        struct iovec aIOVecs[BATCH_SIZE];
        struct mmsghdr aMsgVec[BATCH_SIZE];
        sockaddr_in aAddrs[BATCH_SIZE];

        // Initialize the aIOVecs and aMsgVec arrays. These are used by the recvmmsg function.
        memset(aMsgVec, 0, sizeof(aMsgVec));
        for (size_t siIter = 0; siIter < BATCH_SIZE; siIter++)
        {
            aIOVecs[siIter].iov_base            = aDataBatch[siIter].data();
            aIOVecs[siIter].iov_len             = aDataBatch[siIter].size();
            aMsgVec[siIter].msg_hdr.msg_iov     = &aIOVecs[siIter];
            aMsgVec[siIter].msg_hdr.msg_iovlen  = 1;
            aMsgVec[siIter].msg_hdr.msg_name    = &aAddrs[siIter];
            aMsgVec[siIter].msg_hdr.msg_namelen = sizeof(sockaddr_in);
        }

        // Acquire a write lock on the socket receive mutex to protect the socket, which is shared between threads, but not thread-safe.
        std::unique_lock<std::mutex> lkSocketReceiveLock(m_muSocketReceiveMutex);

        // Receive a batch of packets.
        int nRet = recvmmsg(m_nUDPSocket, aMsgVec, BATCH_SIZE, MSG_DONTWAIT, nullptr);
        if (nRet < 0)
        {
            // Still waiting for data or connection return without error.
            if (errno != EAGAIN && errno != EWOULDBLOCK)
            {
                perror("Failed to receive data from UDP socket using recvmmsg.");
            }
            return;
        }
        // Release the lock.
        lkSocketReceiveLock.unlock();

        // Process each received packet.
        for (int nIter = 0; nIter < nRet; nIter++)
        {
            // Get the data and client address from the received packet.
            size_t siBytesReceived = aMsgVec[nIter].msg_len;
            if (siBytesReceived < ROVECOMM_PACKET_HEADER_SIZE)
            {
                throw std::runtime_error("Not enough data to parse RoveCommPacket header.");
            }
            std::span<uint8_t> spData{aDataBatch[nIter].data(), siBytesReceived};
            sockaddr_in& saClientAddr = aAddrs[nIter];
            // Extract the data id and data type from the packet.
            uint16_t unDataId             = (static_cast<uint16_t>(spData[1]) << 8) | static_cast<uint16_t>(spData[2]);
            manifest::DataTypes eDataType = static_cast<manifest::DataTypes>(spData[5]);

            switch (eDataType)
            {
                case manifest::DataTypes::UINT8_T: ProcessPacket<uint8_t>(spData, saClientAddr); break;
                case manifest::DataTypes::INT8_T: ProcessPacket<int8_t>(spData, saClientAddr); break;
                case manifest::DataTypes::UINT16_T: ProcessPacket<uint16_t>(spData, saClientAddr); break;
                case manifest::DataTypes::INT16_T: ProcessPacket<int16_t>(spData, saClientAddr); break;
                case manifest::DataTypes::UINT32_T: ProcessPacket<uint32_t>(spData, saClientAddr); break;
                case manifest::DataTypes::INT32_T: ProcessPacket<int32_t>(spData, saClientAddr); break;
                case manifest::DataTypes::FLOAT_T: ProcessPacket<float>(spData, saClientAddr); break;
                case manifest::DataTypes::DOUBLE_T: ProcessPacket<double>(spData, saClientAddr); break;
                case manifest::DataTypes::CHAR: ProcessPacket<char>(spData, saClientAddr); break;
            }
        }
#endif
    }

    /******************************************************************************
     * @brief Add a subscriber to the list of subscribers. The subscriber will
     *        receive all packets that are sent to the specified IP address and port.
     *
     * @param t - The IP address of the subscriber.
     * @param nPort - The port that the subscriber is listening on.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-08
     ******************************************************************************/
    void RoveCommUDP::AddSubscriber(const std::string& t, const int nPort)
    {
        if (seSubscribers.size() < ROVECOMM_ETHERNET_UDP_MAX_SUBSCRIBERS)
        {
            // Add new subscriber
            seSubscribers.insert({t, nPort});
        }
    }

    /******************************************************************************
     * @brief Remove a subscriber from the list of subscribers. The subscriber will
     *        no longer receive packets that are sent to the specified IP address and
     *        port.
     *
     * @param t - The IP address of the subscriber.
     * @param nPort - The port that the subscriber is listening on.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-08
     ******************************************************************************/
    void RoveCommUDP::RemoveSubscriber(const std::string& t, const int nPort)
    {
        // Find and remove the subscriber
        seSubscribers.erase({t, nPort});
    }

    /******************************************************************************
     * @brief Initialize the thread that will continuously receive UDP packets and
     *        invoke the appropriate callback function. This is a component of the
     *        AutonomyThread class and is called when the thread is started.
     *
     * @note This function is not intended to be called directly. It is called from
     *       the AutonomyThread class. After the thread has been started.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    void RoveCommUDP::ThreadedContinuousCode()
    {
        // Start the thread pool to receive multiple packets at once.
        this->RunDetachedPool(10, 5);
        // Wait for thread pool to finish.
        this->JoinPool();
    }

    /******************************************************************************
     * @brief This method holds the code that is ran in the thread pool started by
     *        the ThreadedLinearCode() method. It currently does nothing and is not
     *        needed in the current implementation of the RoveCommUDP class.
     *
     * @note This function is not intended to be called directly. It is called from
     *       the AutonomyThread class. After the thread pool has been started.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    void RoveCommUDP::PooledLinearCode()
    {
        ReceiveAndCallback();
    }

    /******************************************************************************
     * @brief Close the UDP socket. This method is called when the RoveCommUDP
     *        object is destroyed. Or when the user calls the CloseUDPSocket method.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    void RoveCommUDP::Close()
    {
        // Check if the socket is open
        if (m_nUDPSocket != -1)
        {
            // Stop the thread
            RequestStop();
            Join();

            // Close the socket
            CLOSE_SOCKET(m_nUDPSocket);

#if defined(__ROVECOMM_WINDOWS_MODE__) && __ROVECOMM_WINDOWS_MODE__ == 1
            WSACleanup();
#endif
        }
    }

    // Explicitly define template function types
    template ssize_t RoveCommUDP::Send<uint8_t>(const RoveCommPacket<uint8_t>&, const manifest::AddressEntry&, int);
    template void RoveCommUDP::On<uint8_t>(const uint16_t, std::function<void(const RoveCommPacket<uint8_t>&)>);
    template void RoveCommUDP::Clear<uint8_t>(const uint16_t);

    template ssize_t RoveCommUDP::Send<int8_t>(const RoveCommPacket<int8_t>&, const manifest::AddressEntry&, int);
    template void RoveCommUDP::On<int8_t>(const uint16_t, std::function<void(const RoveCommPacket<int8_t>&)>);
    template void RoveCommUDP::Clear<int8_t>(const uint16_t);

    template ssize_t RoveCommUDP::Send<uint16_t>(const RoveCommPacket<uint16_t>&, const manifest::AddressEntry&, int);
    template void RoveCommUDP::On<uint16_t>(const uint16_t, std::function<void(const RoveCommPacket<uint16_t>&)>);
    template void RoveCommUDP::Clear<uint16_t>(const uint16_t);

    template ssize_t RoveCommUDP::Send<int16_t>(const RoveCommPacket<int16_t>&, const manifest::AddressEntry&, int);
    template void RoveCommUDP::On<int16_t>(const uint16_t, std::function<void(const RoveCommPacket<int16_t>&)>);
    template void RoveCommUDP::Clear<int16_t>(const uint16_t);

    template ssize_t RoveCommUDP::Send<uint32_t>(const RoveCommPacket<uint32_t>&, const manifest::AddressEntry&, int);
    template void RoveCommUDP::On<uint32_t>(const uint16_t, std::function<void(const RoveCommPacket<uint32_t>&)>);
    template void RoveCommUDP::Clear<uint32_t>(const uint16_t);

    template ssize_t RoveCommUDP::Send<int32_t>(const RoveCommPacket<int32_t>&, const manifest::AddressEntry&, int);
    template void RoveCommUDP::On<int32_t>(const uint16_t, std::function<void(const RoveCommPacket<int32_t>&)>);
    template void RoveCommUDP::Clear<int32_t>(const uint16_t);

    template ssize_t RoveCommUDP::Send<float>(const RoveCommPacket<float>&, const manifest::AddressEntry&, int);
    template void RoveCommUDP::On<float>(const uint16_t, std::function<void(const RoveCommPacket<float>&)>);
    template void RoveCommUDP::Clear<float>(const uint16_t);

    template ssize_t RoveCommUDP::Send<double>(const RoveCommPacket<double>&, const manifest::AddressEntry&, int);
    template void RoveCommUDP::On<double>(const uint16_t, std::function<void(const RoveCommPacket<double>&)>);
    template void RoveCommUDP::Clear<double>(const uint16_t);

    template ssize_t RoveCommUDP::Send<char>(const RoveCommPacket<char>&, const manifest::AddressEntry&, int);
    template void RoveCommUDP::On<char>(const uint16_t, std::function<void(const RoveCommPacket<char>&)>);
    template void RoveCommUDP::Clear<char>(const uint16_t);

}    // namespace rovecomm
