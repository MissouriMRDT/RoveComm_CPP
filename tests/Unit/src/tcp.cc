/******************************************************************************
 * @brief Unit test for TCP functionality in RoveComm.
 *
 * @file tcp.cc
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
#include <gtest/gtest.h>

/// \endcond

/******************************************************************************
 * @brief Test Initialization of a TCP Socket.
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-08
 ******************************************************************************/
TEST(RoveCommTCP, InitSocket)
{
    // Run the test via the RunTimedTest function to allow for retries and timeouts.
    testutils::RunTimedTest(
        []()
        {
            // Create RoveComm Node
            rovecomm::RoveCommTCP RoveCommTCPNode;

            bool bInitSuccess = false;

            // Give the node three chances to initialize the socket
            // Since the socket is bound to a specific port, it may take a few tries to find an available port
            for (int i = 0; i < 3; ++i)
            {
                if (RoveCommTCPNode.Init("127.0.0.1", 12000))
                {
                    bInitSuccess = true;
                    break;
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }

            // Initialize the TCP node
            EXPECT_TRUE(bInitSuccess);

            // Close the socket
            RoveCommTCPNode.Close();
        },
        3,         // 3 total attempts
        30000);    // 30 second timeout (30,000 ms)
}

/******************************************************************************
 * @brief Test Sending a TCP Packet.
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-08
 ******************************************************************************/
TEST(RoveCommTCP, SendTCPPacket)
{
    // Run the test via the RunTimedTest function to allow for retries and timeouts.
    testutils::RunTimedTest(
        []()
        {
            // Create RoveComm Nodes
            rovecomm::RoveCommTCP RoveCommTCPNode;

            // Give the node three chances to initialize the socket
            // Since the socket is bound to a specific port, it may take a few tries to find an available port
            for (int i = 0; i < 3; ++i)
            {
                if (RoveCommTCPNode.Init("127.0.0.1", 12001))
                {
                    break;
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }

            // Create RoveCommPacket
            rovecomm::RoveCommPacket<uint8_t> stPacket{manifest::Autonomy::Commands::STARTAUTONOMY.DATA_ID, {1}};

            // Send the packet to the localhost
            ssize_t siBytesSent = RoveCommTCPNode.Send<uint8_t>(stPacket, "127.0.0.1", 12001);

            // Check if the packet successfully sent
            EXPECT_EQ(siBytesSent, 7);

            // Close the socket
            RoveCommTCPNode.Close();
        },
        3,         // 3 total attempts
        30000);    // 30 second timeout (30,000 ms)
}

/******************************************************************************
 * @brief Test Adding and Removing a TCP Callback.
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-08
 ******************************************************************************/
TEST(RoveCommTCP, CallbackInvoked)
{
    // Run the test via the RunTimedTest function to allow for retries and timeouts.
    testutils::RunTimedTest(
        []()
        {
            // Create RoveComm Nodes
            rovecomm::RoveCommTCP RoveCommTCPNode;

            // Give the node three chances to initialize the socket
            // Since the socket is bound to a specific port, it may take a few tries to find an available port
            for (int i = 0; i < 3; ++i)
            {
                if (RoveCommTCPNode.Init("127.0.0.1", 12002))
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

            std::vector<int32_t> vExpectedData = {-100, -1, 5555};

            // Add a callback function for data ID 1100
            RoveCommTCPNode.On<int32_t>(1100,
                                        [&](const rovecomm::RoveCommPacket<int32_t>& packet)
                                        {
                                            // Set the flag to true to indicate that the callback was invoked
                                            bCallbackInvoked = true;

                                            // Assertions to verify the behavior of the callback function
                                            EXPECT_EQ(packet.unDataId, 1100);                             // Check the data ID
                                            EXPECT_EQ(packet.GetDataCount(), 3);                          // Check the data count
                                            EXPECT_EQ(packet.eDataType, manifest::DataTypes::INT32_T);    // Check the data type

                                            for (size_t i = 0; i < packet.vData.size(); i++)
                                            {
                                                EXPECT_EQ(packet.vData[i], vExpectedData[i]);    // Check the data
                                            }
                                        });

            // Simulate receiving a packet with data ID 1100
            // Create RoveCommPacket
            rovecomm::RoveCommPacket<int32_t> stPacket{1100, {}};

            for (int32_t nData : vExpectedData)
            {
                stPacket.vData.push_back(nData);    // Sample data
            }

            // Pack the packet
            std::vector<uint8_t> vData = rovecomm::PackPacket(stPacket);

            // Process the received packet (simulate callback invocation)
            RoveCommTCPNode.ProcessPacket<int32_t>(vData);

            // Check if the callback was invoked
            EXPECT_TRUE(bCallbackInvoked);

            // Close the socket
            RoveCommTCPNode.Close();
        },
        3,         // 3 total attempts
        30000);    // 30 second timeout (30,000 ms)
}

TEST(RoveCommTCP, CallbacksBelongToTheirNodeAndOffRemovesOne)
{
    // Run the test via the RunTimedTest function to allow for retries and timeouts.
    testutils::RunTimedTest(
        []()
        {
            // Two nodes in one process. Neither needs a socket: packets are fed straight to ProcessPacket.
            rovecomm::RoveCommTCP RoveCommTCPNodeA;
            rovecomm::RoveCommTCP RoveCommTCPNodeB;

            const uint16_t unTestDataId = 1402;
            int nCallsFirst             = 0;
            int nCallsSecond            = 0;
            std::vector<uint8_t> vData  = rovecomm::PackPacket(rovecomm::RoveCommPacket<int16_t>{.unDataId = unTestDataId, .vData = {-7, 7}});

            // Two callbacks for the same data ID, both on node A.
            rovecomm::CallbackHandle stFirst = RoveCommTCPNodeA.On<int16_t>(unTestDataId, [&](const rovecomm::RoveCommPacket<int16_t>&) { ++nCallsFirst; });
            RoveCommTCPNodeA.On<int16_t>(unTestDataId, [&](const rovecomm::RoveCommPacket<int16_t>&) { ++nCallsSecond; });

            // A packet received by node B must not reach node A's callbacks.
            RoveCommTCPNodeB.ProcessPacket<int16_t>(vData);
            EXPECT_EQ(nCallsFirst + nCallsSecond, 0) << "Node B invoked callbacks registered on node A";

            // Node A invokes both, then only the second once Off() removes the first.
            RoveCommTCPNodeA.ProcessPacket<int16_t>(vData);
            RoveCommTCPNodeA.Off(stFirst);
            RoveCommTCPNodeA.ProcessPacket<int16_t>(vData);
            EXPECT_EQ(nCallsFirst, 1) << "Off() left its callback registered";
            EXPECT_EQ(nCallsSecond, 2) << "Off() removed a different callback";

            // Clear() removes the rest.
            RoveCommTCPNodeA.Clear<int16_t>(unTestDataId);
            RoveCommTCPNodeA.ProcessPacket<int16_t>(vData);
            EXPECT_EQ(nCallsSecond, 2) << "Clear() left a callback registered";
        },
        3,         // 3 total attempts
        30000);    // 30 second timeout (30,000 ms)
}
