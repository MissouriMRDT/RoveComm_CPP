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
            RoveCommData data;
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
        CloseUDPSocket();
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
    bool RoveCommUDP::InitUDPSocket(int nPort)
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
    ssize_t RoveCommUDP::SendUDPPacket(const RoveCommPacket<T>& stPacket, const char* cIPAddress, int nPort)
    {
        // Pack the RoveCommPacket into a RoveCommData structure
        RoveCommData stData = PackPacket(stPacket);
        // Get size of data not including the data not filled. (the null/zero data in RoveCommData)
        size_t siDataSize = ROVECOMM_PACKET_HEADER_SIZE + (sizeof(T) * stPacket.unDataCount);

#if defined(__ROVECOMM_WINDOWS_MODE__) && __ROVECOMM_WINDOWS_MODE__ == 1
        // Setup the base UDP client address
        struct sockaddr_in saUDPClientAddr;
        memset(&saUDPClientAddr, 0, sizeof(saUDPClientAddr));
        saUDPClientAddr.sin_family = AF_INET;

        // Send the packet to all subscribers
        for (const SubscriberInfo& stSubscriber : vSubscribers)
        {
            // Assemble client address.
            saUDPClientAddr.sin_port = htons(stSubscriber.nPort);
            inet_pton(AF_INET, stSubscriber.szIPAddress.c_str(), &saUDPClientAddr.sin_addr);
            // Acquire a write lock on the socket send mutex to protect the socket, which is shared between threads, but not thread-safe.
            std::unique_lock<std::mutex> lkSocketSendLock(m_muSocketSendMutex);
            // Send data.
            if (sendto(m_nUDPSocket, reinterpret_cast<char*>(&stData), siDataSize, 0, (struct sockaddr*) &saUDPClientAddr, sizeof(saUDPClientAddr)) == -1)
            {
                // Handle and print error message.
                perror("Failed to send data to UDP client socket subscriber.");
            }
            // Release the lock.
            lkSocketSendLock.unlock();
        }

        // Send the packet to the specified IP address and port.
        if (std::strcmp(cIPAddress, "0.0.0.0") && nPort != 0)
        {
            saUDPClientAddr.sin_port = htons(nPort);
            inet_pton(AF_INET, cIPAddress, &saUDPClientAddr.sin_addr);

            // Acquire a write lock on the socket send mutex to protect the socket, which is shared between threads, but not thread-safe.
            std::unique_lock<std::mutex> lkSocketSendLock(m_muSocketSendMutex);
            return sendto(m_nUDPSocket, reinterpret_cast<char*>(&stData), siDataSize, 0, (struct sockaddr*) &saUDPClientAddr, sizeof(saUDPClientAddr));
        }

        return -1;
#else
        // Use sendmmsg to send the same packet to multiple destinations in one call.
        std::vector<sockaddr_in> vDestinations;
        // Add all subscribers.
        for (const SubscriberInfo& stSubscriber : vSubscribers)
        {
            // Assemble client address.
            sockaddr_in stdAddr{};
            stdAddr.sin_family = AF_INET;
            stdAddr.sin_port   = htons(stSubscriber.nPort);
            // Convert the IP address to binary form.
            inet_pton(AF_INET, stSubscriber.szIPAddress.c_str(), &stdAddr.sin_addr);
            // Add the subscriber to the list of destinations.
            vDestinations.push_back(stdAddr);
        }
        // Add the specified destination if provided.
        if (std::strcmp(cIPAddress, "0.0.0.0") != 0 && nPort != 0)
        {
            // Assemble client address.
            sockaddr_in stdAddr{};
            stdAddr.sin_family = AF_INET;
            stdAddr.sin_port   = htons(nPort);
            // Convert the IP address to binary form.
            inet_pton(AF_INET, cIPAddress, &stdAddr.sin_addr);
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
            vIOVecs[siIter].iov_base = reinterpret_cast<char*>(&stData);
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
    void RoveCommUDP::AddUDPCallback(std::function<void(const RoveCommPacket<T>&, const sockaddr_in&)> fnCallback, const uint16_t& unCondition)
    {
        // Acquire a write lock to protect the callback vectors.
        std::unique_lock<std::shared_mutex> lkCallbackLock(m_muCallbackMutex);

        // Add the callback function to the vector of UDP callbacks for the specified data type
        if constexpr (std::is_same_v<T, uint8_t>)
        {
            // Add the callback function to the vector of uint8_t callbacks
            udp::vUInt8Callbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
        else if constexpr (std::is_same_v<T, int8_t>)
        {
            // Add the callback function to the vector of int8_t callbacks
            udp::vInt8Callbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
        else if constexpr (std::is_same_v<T, uint16_t>)
        {
            // Add the callback function to the vector of uint16_t callbacks
            udp::vUInt16Callbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
        else if constexpr (std::is_same_v<T, int16_t>)
        {
            // Add the callback function to the vector of int16_t callbacks
            udp::vInt16Callbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
        else if constexpr (std::is_same_v<T, uint32_t>)
        {
            // Add the callback function to the vector of uint32_t callbacks
            udp::vUInt32Callbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
        else if constexpr (std::is_same_v<T, int32_t>)
        {
            // Add the callback function to the vector of int32_t callbacks
            udp::vInt32Callbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
        else if constexpr (std::is_same_v<T, float>)
        {
            // Add the callback function to the vector of float callbacks
            udp::vFloatCallbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            // Add the callback function to the vector of double callbacks
            udp::vDoubleCallbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
        else if constexpr (std::is_same_v<T, char>)
        {
            // Add the callback function to the vector of char callbacks
            udp::vCharCallbacks.push_back(std::make_tuple(fnCallback, unCondition));
        }
    }

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
    void RoveCommUDP::RemoveUDPCallback(std::function<void(const RoveCommPacket<T>&, const sockaddr_in&)> fnCallback)
    {
        // Acquire a write lock to protect the callback vectors.
        std::unique_lock<std::shared_mutex> lkCallbackLock(m_muCallbackMutex);

        // Remove the callback function from the vector of UDP callbacks for the specified data type
        if constexpr (std::is_same_v<T, uint8_t>)
        {
            // Remove the callback function from the vector of uint8_t callbacks
            udp::vUInt8Callbacks.erase(std::remove_if(udp::vUInt8Callbacks.begin(),
                                                      udp::vUInt8Callbacks.end(),
                                                      [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                       udp::vUInt8Callbacks.end());
        }
        else if constexpr (std::is_same_v<T, int8_t>)
        {
            // Remove the callback function from the vector of int8_t callbacks
            udp::vInt8Callbacks.erase(std::remove_if(udp::vInt8Callbacks.begin(),
                                                     udp::vInt8Callbacks.end(),
                                                     [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                      udp::vInt8Callbacks.end());
        }
        else if constexpr (std::is_same_v<T, uint16_t>)
        {
            // Remove the callback function from the vector of uint16_t callbacks
            udp::vUInt16Callbacks.erase(std::remove_if(udp::vUInt16Callbacks.begin(),
                                                       udp::vUInt16Callbacks.end(),
                                                       [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                        udp::vUInt16Callbacks.end());
        }
        else if constexpr (std::is_same_v<T, int16_t>)
        {
            // Remove the callback function from the vector of int16_t callbacks
            udp::vInt16Callbacks.erase(std::remove_if(udp::vInt16Callbacks.begin(),
                                                      udp::vInt16Callbacks.end(),
                                                      [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                       udp::vInt16Callbacks.end());
        }
        else if constexpr (std::is_same_v<T, uint32_t>)
        {
            // Remove the callback function from the vector of uint32_t callbacks
            udp::vUInt32Callbacks.erase(std::remove_if(udp::vUInt32Callbacks.begin(),
                                                       udp::vUInt32Callbacks.end(),
                                                       [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                        udp::vUInt32Callbacks.end());
        }
        else if constexpr (std::is_same_v<T, int32_t>)
        {
            // Remove the callback function from the vector of int32_t callbacks
            udp::vInt32Callbacks.erase(std::remove_if(udp::vInt32Callbacks.begin(),
                                                      udp::vInt32Callbacks.end(),
                                                      [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                       udp::vInt32Callbacks.end());
        }
        else if constexpr (std::is_same_v<T, float>)
        {
            // Remove the callback function from the vector of float callbacks
            udp::vFloatCallbacks.erase(std::remove_if(udp::vFloatCallbacks.begin(),
                                                      udp::vFloatCallbacks.end(),
                                                      [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                       udp::vFloatCallbacks.end());
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            // Remove the callback function from the vector of double callbacks
            udp::vDoubleCallbacks.erase(std::remove_if(udp::vDoubleCallbacks.begin(),
                                                       udp::vDoubleCallbacks.end(),
                                                       [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                        udp::vDoubleCallbacks.end());
        }
        else if constexpr (std::is_same_v<T, char>)
        {
            // Remove the callback function from the vector of char callbacks
            udp::vCharCallbacks.erase(std::remove_if(udp::vCharCallbacks.begin(),
                                                     udp::vCharCallbacks.end(),
                                                     [&](const auto& tuple) { return std::get<0>(tuple).target_type() == fnCallback.target_type(); }),
                                      udp::vCharCallbacks.end());
        }
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
     * @param stData - The received RoveCommData that is to be processed.
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
    void RoveCommUDP::ProcessPacket(const RoveCommData& stData,
                                    const std::vector<std::tuple<std::function<void(const RoveCommPacket<T>&, const sockaddr_in&)>, uint32_t>>& vCallbacks,
                                    const sockaddr_in& saClientAddr)
    {
        // Unpack the received data into a RoveCommPacket
        RoveCommPacket<T> stPacket = UnpackData<T>(stData);

        // Create a SubscriberInfo struct to store the client address
        SubscriberInfo stSubscriber;
        stSubscriber.szIPAddress = inet_ntoa(saClientAddr.sin_addr);
        stSubscriber.nPort       = ntohs(saClientAddr.sin_port);

        // Check if the received packet is a subscribe or unsubscribe packet
        if (stPacket.unDataId == manifest::System::SUBSCRIBE_DATA_ID)
        {
            AddSubscriber(stSubscriber.szIPAddress, stSubscriber.nPort);
        }
        else if (stPacket.unDataId == manifest::System::UNSUBSCRIBE_DATA_ID)
        {
            RemoveSubscriber(stSubscriber.szIPAddress, stSubscriber.nPort);
        }

        // Acquire a read lock to protect the callback vectors.
        std::shared_lock<std::shared_mutex> lkCallbackLock(m_muCallbackMutex);

        // Invoke registered callbacks
        for (const std::tuple<std::function<void(const RoveCommPacket<T>&, const sockaddr_in&)>, uint32_t>& tpCallbackInfo : vCallbacks)
        {
            const std::function<void(const RoveCommPacket<T>&, const sockaddr_in&)>& fnCallback = std::get<0>(tpCallbackInfo);
            const uint32_t& unCondition                                                         = std::get<1>(tpCallbackInfo);

            if (unCondition == stPacket.unDataId)
            {
                fnCallback(stPacket, saClientAddr);
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
    void RoveCommUDP::ReceiveUDPPacketAndCallback()
    {
#if defined(__ROVECOMM_WINDOWS_MODE__) && __ROVECOMM_WINDOWS_MODE__ == 1
        // Create a batch of RecvContext structures to receive multiple packets at once.
        constexpr size_t BATCH_SIZE = 32;
        RecvContext rcContexts[BATCH_SIZE];

        // Post BATCH_SIZE asynchronous receives.
        for (size_t siIter = 0; siIter < BATCH_SIZE; siIter++)
        {
            ZeroMemory(&rcContexts[siIter].overlapped, sizeof(OVERLAPPED));
            rcContexts[siIter].wsabuf.buf = reinterpret_cast<char*>(&rcContexts[siIter].data);
            rcContexts[siIter].wsabuf.len = sizeof(RoveCommData);
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
            RoveCommData& stData      = pContext->data;
            sockaddr_in& saClientAddr = pContext->addr;

            // Extract data id and data type from the packet.
            uint16_t unDataId             = (static_cast<uint16_t>(stData.unBytes[1]) << 8) | static_cast<uint16_t>(stData.unBytes[2]);
            manifest::DataTypes eDataType = static_cast<manifest::DataTypes>(stData.unBytes[5]);

            switch (eDataType)
            {
                case manifest::DataTypes::UINT8_T: ProcessPacket<uint8_t>(stData, udp::vUInt8Callbacks, saClientAddr); break;
                case manifest::DataTypes::INT8_T: ProcessPacket<int8_t>(stData, udp::vInt8Callbacks, saClientAddr); break;
                case manifest::DataTypes::UINT16_T: ProcessPacket<uint16_t>(stData, udp::vUInt16Callbacks, saClientAddr); break;
                case manifest::DataTypes::INT16_T: ProcessPacket<int16_t>(stData, udp::vInt16Callbacks, saClientAddr); break;
                case manifest::DataTypes::UINT32_T: ProcessPacket<uint32_t>(stData, udp::vUInt32Callbacks, saClientAddr); break;
                case manifest::DataTypes::INT32_T: ProcessPacket<int32_t>(stData, udp::vInt32Callbacks, saClientAddr); break;
                case manifest::DataTypes::FLOAT_T: ProcessPacket<float>(stData, udp::vFloatCallbacks, saClientAddr); break;
                case manifest::DataTypes::DOUBLE_T: ProcessPacket<double>(stData, udp::vDoubleCallbacks, saClientAddr); break;
                case manifest::DataTypes::CHAR: ProcessPacket<char>(stData, udp::vCharCallbacks, saClientAddr); break;
            }

            // Re-post the asynchronous receive for this context.
            ZeroMemory(&pContext->overlapped, sizeof(OVERLAPPED));
            pContext->wsabuf.buf = reinterpret_cast<char*>(&pContext->data);
            pContext->wsabuf.len = sizeof(RoveCommData);
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
        RoveCommData aDataBatch[BATCH_SIZE];
        struct iovec aIOVecs[BATCH_SIZE];
        struct mmsghdr aMsgVec[BATCH_SIZE];
        sockaddr_in aAddrs[BATCH_SIZE];

        // Initialize the aIOVecs and aMsgVec arrays. These are used by the recvmmsg function.
        memset(aMsgVec, 0, sizeof(aMsgVec));
        for (size_t siIter = 0; siIter < BATCH_SIZE; siIter++)
        {
            aIOVecs[siIter].iov_base            = &aDataBatch[siIter];
            aIOVecs[siIter].iov_len             = sizeof(RoveCommData);
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
            RoveCommData& stData      = aDataBatch[nIter];
            sockaddr_in& saClientAddr = aAddrs[nIter];

            // Extract the data id and data type from the packet.
            uint16_t unDataId             = (static_cast<uint16_t>(stData.unBytes[1]) << 8) | static_cast<uint16_t>(stData.unBytes[2]);
            manifest::DataTypes eDataType = static_cast<manifest::DataTypes>(stData.unBytes[5]);

            switch (eDataType)
            {
                case manifest::DataTypes::UINT8_T: ProcessPacket<uint8_t>(stData, udp::vUInt8Callbacks, saClientAddr); break;
                case manifest::DataTypes::INT8_T: ProcessPacket<int8_t>(stData, udp::vInt8Callbacks, saClientAddr); break;
                case manifest::DataTypes::UINT16_T: ProcessPacket<uint16_t>(stData, udp::vUInt16Callbacks, saClientAddr); break;
                case manifest::DataTypes::INT16_T: ProcessPacket<int16_t>(stData, udp::vInt16Callbacks, saClientAddr); break;
                case manifest::DataTypes::UINT32_T: ProcessPacket<uint32_t>(stData, udp::vUInt32Callbacks, saClientAddr); break;
                case manifest::DataTypes::INT32_T: ProcessPacket<int32_t>(stData, udp::vInt32Callbacks, saClientAddr); break;
                case manifest::DataTypes::FLOAT_T: ProcessPacket<float>(stData, udp::vFloatCallbacks, saClientAddr); break;
                case manifest::DataTypes::DOUBLE_T: ProcessPacket<double>(stData, udp::vDoubleCallbacks, saClientAddr); break;
                case manifest::DataTypes::CHAR: ProcessPacket<char>(stData, udp::vCharCallbacks, saClientAddr); break;
            }
        }
#endif
    }

    /******************************************************************************
     * @brief Add a subscriber to the list of subscribers. The subscriber will
     *        receive all packets that are sent to the specified IP address and port.
     *
     * @param szIPAddress - The IP address of the subscriber.
     * @param nPort - The port that the subscriber is listening on.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-08
     ******************************************************************************/
    void RoveCommUDP::AddSubscriber(const std::string& szIPAddress, const int& nPort)
    {
        if (vSubscribers.size() < ROVECOMM_ETHERNET_UDP_MAX_SUBSCRIBERS)
        {
            // Check if the subscriber is already in the list
            for (const auto& subscriber : vSubscribers)
            {
                if (subscriber.szIPAddress == szIPAddress && subscriber.nPort == nPort)
                {
                    return;    // Subscriber already exists, no need to add again
                }
            }

            // Add new subscriber
            vSubscribers.push_back({szIPAddress, nPort});
        }
    }

    /******************************************************************************
     * @brief Remove a subscriber from the list of subscribers. The subscriber will
     *        no longer receive packets that are sent to the specified IP address and
     *        port.
     *
     * @param szIPAddress - The IP address of the subscriber.
     * @param nPort - The port that the subscriber is listening on.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-08
     ******************************************************************************/
    void RoveCommUDP::RemoveSubscriber(const std::string& szIPAddress, const int& nPort)
    {
        // Find and remove the subscriber
        vSubscribers.erase(
            std::remove_if(vSubscribers.begin(), vSubscribers.end(), [&](const SubscriberInfo& info) { return info.szIPAddress == szIPAddress && info.nPort == nPort; }),
            vSubscribers.end());
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
        ReceiveUDPPacketAndCallback();
    }

    /******************************************************************************
     * @brief Close the UDP socket. This method is called when the RoveCommUDP
     *        object is destroyed. Or when the user calls the CloseUDPSocket method.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    void RoveCommUDP::CloseUDPSocket()
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
    template ssize_t RoveCommUDP::SendUDPPacket<uint8_t>(const RoveCommPacket<uint8_t>&, const char*, int);
    template void RoveCommUDP::AddUDPCallback<uint8_t>(std::function<void(const RoveCommPacket<uint8_t>&, const sockaddr_in&)>, const uint16_t&);
    template void RoveCommUDP::RemoveUDPCallback<uint8_t>(std::function<void(const RoveCommPacket<uint8_t>&, const sockaddr_in&)>);

    template ssize_t RoveCommUDP::SendUDPPacket<int8_t>(const RoveCommPacket<int8_t>&, const char*, int);
    template void RoveCommUDP::AddUDPCallback<int8_t>(std::function<void(const RoveCommPacket<int8_t>&, const sockaddr_in&)>, const uint16_t&);
    template void RoveCommUDP::RemoveUDPCallback<int8_t>(std::function<void(const RoveCommPacket<int8_t>&, const sockaddr_in&)>);

    template ssize_t RoveCommUDP::SendUDPPacket<uint16_t>(const RoveCommPacket<uint16_t>&, const char*, int);
    template void RoveCommUDP::AddUDPCallback<uint16_t>(std::function<void(const RoveCommPacket<uint16_t>&, const sockaddr_in&)>, const uint16_t&);
    template void RoveCommUDP::RemoveUDPCallback<uint16_t>(std::function<void(const RoveCommPacket<uint16_t>&, const sockaddr_in&)>);

    template ssize_t RoveCommUDP::SendUDPPacket<int16_t>(const RoveCommPacket<int16_t>&, const char*, int);
    template void RoveCommUDP::AddUDPCallback<int16_t>(std::function<void(const RoveCommPacket<int16_t>&, const sockaddr_in&)>, const uint16_t&);
    template void RoveCommUDP::RemoveUDPCallback<int16_t>(std::function<void(const RoveCommPacket<int16_t>&, const sockaddr_in&)>);

    template ssize_t RoveCommUDP::SendUDPPacket<uint32_t>(const RoveCommPacket<uint32_t>&, const char*, int);
    template void RoveCommUDP::AddUDPCallback<uint32_t>(std::function<void(const RoveCommPacket<uint32_t>&, const sockaddr_in&)>, const uint16_t&);
    template void RoveCommUDP::RemoveUDPCallback<uint32_t>(std::function<void(const RoveCommPacket<uint32_t>&, const sockaddr_in&)>);

    template ssize_t RoveCommUDP::SendUDPPacket<int32_t>(const RoveCommPacket<int32_t>&, const char*, int);
    template void RoveCommUDP::AddUDPCallback<int32_t>(std::function<void(const RoveCommPacket<int32_t>&, const sockaddr_in&)>, const uint16_t&);
    template void RoveCommUDP::RemoveUDPCallback<int32_t>(std::function<void(const RoveCommPacket<int32_t>&, const sockaddr_in&)>);

    template ssize_t RoveCommUDP::SendUDPPacket<float>(const RoveCommPacket<float>&, const char*, int);
    template void RoveCommUDP::AddUDPCallback<float>(std::function<void(const RoveCommPacket<float>&, const sockaddr_in&)>, const uint16_t&);
    template void RoveCommUDP::RemoveUDPCallback<float>(std::function<void(const RoveCommPacket<float>&, const sockaddr_in&)>);

    template ssize_t RoveCommUDP::SendUDPPacket<double>(const RoveCommPacket<double>&, const char*, int);
    template void RoveCommUDP::AddUDPCallback<double>(std::function<void(const RoveCommPacket<double>&, const sockaddr_in&)>, const uint16_t&);
    template void RoveCommUDP::RemoveUDPCallback<double>(std::function<void(const RoveCommPacket<double>&, const sockaddr_in&)>);

    template ssize_t RoveCommUDP::SendUDPPacket<char>(const RoveCommPacket<char>&, const char*, int);
    template void RoveCommUDP::AddUDPCallback<char>(std::function<void(const RoveCommPacket<char>&, const sockaddr_in&)>, const uint16_t&);
    template void RoveCommUDP::RemoveUDPCallback<char>(std::function<void(const RoveCommPacket<char>&, const sockaddr_in&)>);

}    // namespace rovecomm
