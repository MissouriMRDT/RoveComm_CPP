/******************************************************************************
 * @brief Unit test for UDP functionality in RoveComm.
 *
 * @file udp.cc
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-07
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#include "../../../src/RoveComm/RoveComm.h"
#include "../../TestUtils.h"

/// \cond
#include <array>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <gtest/gtest.h>
#include <thread>

/// \endcond

/******************************************************************************
 * @brief Test Initialization of a UDP Socket.
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-08
 ******************************************************************************/
TEST(RoveCommUDP, InitSocket)
{
    // Run the test via the RunTimedTest function to allow for retries and timeouts.
    testutils::RunTimedTest(
        []()
        {
            // Create RoveComm Node
            rovecomm::RoveCommUDP pRoveCommUDP_Node;

            // Flag to check if the initialization was successful
            bool bInitSuccess = false;

            // Give the node three chances to initialize the socket
            // Since the socket is bound to a specific port, it may take a few tries to find an available port
            for (int i = 0; i < 3; ++i)
            {
                // Initialize the UDP node
                if (pRoveCommUDP_Node.InitUDPSocket(11000))
                {
                    bInitSuccess = true;
                    break;
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }

            // Initialize the UDP node
            EXPECT_TRUE(bInitSuccess);

            // Close the socket
            pRoveCommUDP_Node.CloseUDPSocket();
        },
        3,         // 3 total attempts
        30000);    // 30 second timeout (30,000 ms)
}

/******************************************************************************
 * @brief Test Sending a UDP Packet.
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-08
 ******************************************************************************/
TEST(RoveCommUDP, SendUDPPacket)
{
    // Run the test via the RunTimedTest function to allow for retries and timeouts.
    testutils::RunTimedTest(
        []()
        {
            // Create RoveComm Nodes
            rovecomm::RoveCommUDP pRoveCommUDP_Node;

            // Flag to check if the initialization was successful
            bool bInitSuccess = false;

            // Give the node three chances to initialize the socket
            // Since the socket is bound to a specific port, it may take a few tries to find an available port
            for (int i = 0; i < 3; ++i)
            {
                // Initialize the UDP node
                if (pRoveCommUDP_Node.InitUDPSocket(11001))
                {
                    bInitSuccess = true;
                    break;
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }

            if (bInitSuccess)
            {
                // Create RoveCommPacket
                rovecomm::RoveCommPacket<uint8_t> stPacket;
                stPacket.unDataId    = manifest::Autonomy::COMMANDS.find("STARTAUTONOMY")->second.DATA_ID;
                stPacket.unDataCount = manifest::Autonomy::COMMANDS.find("STARTAUTONOMY")->second.DATA_COUNT;
                stPacket.eDataType   = manifest::Autonomy::COMMANDS.find("STARTAUTONOMY")->second.DATA_TYPE;
                stPacket.vData.push_back(1);

                // Send the packet to the localhost
                ssize_t siBytesSent = pRoveCommUDP_Node.SendUDPPacket<uint8_t>(stPacket, "127.0.0.1", 11001);

                // Check if the packet successfully sent
                EXPECT_EQ(siBytesSent, sizeof(rovecomm::PackPacket<uint8_t>(stPacket)));

                // Close the socket
                pRoveCommUDP_Node.CloseUDPSocket();
            }
            else
            {
                FAIL() << "Failed to initialize the UDP socket";
            }
        },
        3,         // 3 total attempts
        30000);    // 30 second timeout (30,000 ms)
}

/******************************************************************************
 * @brief Test Adding and Removing a UDP Callback.
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-08
 ******************************************************************************/
TEST(RoveCommUDP, CallbackInvoked)
{
    // Run the test via the RunTimedTest function to allow for retries and timeouts.
    testutils::RunTimedTest(
        []()
        {
            // Create RoveComm Nodes
            rovecomm::RoveCommUDP pRoveCommUDP_Node;

            // Give the node three chances to initialize the socket
            // Since the socket is bound to a specific port, it may take a few tries to find an available port
            for (int i = 0; i < 3; ++i)
            {
                if (pRoveCommUDP_Node.InitUDPSocket(11002))
                {
                    break;
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }

            // Flag to check if the callback was invoked
            bool bCallbackInvoked              = false;

            std::vector<uint8_t> vExpectedData = {1};

            // Add a callback function for data ID 1100
            pRoveCommUDP_Node.AddUDPCallback<uint8_t>(
                [&](const rovecomm::RoveCommPacket<uint8_t>& packet, const sockaddr_in& address)
                {
                    // Set the flag to true to indicate that the callback was invoked
                    bCallbackInvoked = true;

                    // Assertions to verify the behavior of the callback function
                    EXPECT_EQ(packet.unDataId, 1100);                             // Check the data ID
                    EXPECT_EQ(packet.unDataCount, 1);                             // Check the data count
                    EXPECT_EQ(packet.eDataType, manifest::DataTypes::UINT8_T);    // Check the data type

                    for (size_t i = 0; i < packet.vData.size(); i++)
                    {
                        EXPECT_EQ(packet.vData[i], vExpectedData[i]);    // Check the data
                    }
                },
                1100);

            // Simulate receiving a packet with data ID 1100
            // Create RoveCommPacket
            rovecomm::RoveCommPacket<uint8_t> stPacket;
            stPacket.unDataId    = 1100;                            // Data ID for testing
            stPacket.unDataCount = 1;                               // Sample data count
            stPacket.eDataType   = manifest::DataTypes::UINT8_T;    // Sample data type

            for (uint8_t data : vExpectedData)
            {
                stPacket.vData.push_back(data);    // Sample data
            }

            // Pack the packet
            rovecomm::RoveCommData stData = rovecomm::PackPacket(stPacket);

            // Set the address to a dummy address
            struct sockaddr_in saUDPClientAddr;
            memset(&saUDPClientAddr, 0, sizeof(saUDPClientAddr));
            saUDPClientAddr.sin_family = AF_INET;
            saUDPClientAddr.sin_port   = htons(11002);
            inet_pton(AF_INET, "127.0.0.1", &saUDPClientAddr.sin_addr);

            // Process the received packet (simulate callback invocation)
            pRoveCommUDP_Node.CallProcessPacket<uint8_t>(stData, rovecomm::udp::vUInt8Callbacks, saUDPClientAddr);

            // Check if the callback was invoked
            EXPECT_TRUE(bCallbackInvoked);

            // Close the socket
            pRoveCommUDP_Node.CloseUDPSocket();
        },
        3,         // 3 total attempts
        30000);    // 30 second timeout (30,000 ms)
}
