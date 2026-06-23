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
            rovecomm::RoveCommUDP RoveCommUDPNode;

            // Flag to check if the initialization was successful
            bool bInitSuccess = false;

            // Give the node three chances to initialize the socket
            // Since the socket is bound to a specific port, it may take a few tries to find an available port
            for (int i = 0; i < 3; ++i)
            {
                // Initialize the UDP node
                if (RoveCommUDPNode.Init(11000))
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
            RoveCommUDPNode.Close();
        },
        3,         // 3 total attempts
        30000);    // 30 second timeout (30,000 ms)
}

/******************************************************************************
 * @brief Test Sending Multiple Data Types via UDP.
 *
 *
 * @author clayjay3 (claytonraycowen@gmail.com)
 * @date 2025-04-21
 ******************************************************************************/
TEST(RoveCommUDP, SendMultipleDataTypes)
{
    // Run the test via the RunTimedTest function to allow for retries and timeouts.
    testutils::RunTimedTest(
        []()
        {
            // Create RoveComm Node
            rovecomm::RoveCommUDP RoveCommUDPNode;

            // Flag to check if the initialization was successful
            bool bInitSuccess = false;

            // Give the node three chances to initialize the socket
            // Since the socket is bound to a specific port, it may take a few tries to find an available port
            for (int nI = 0; nI < 3; ++nI)
            {
                // Initialize the UDP node
                if (RoveCommUDPNode.Init(11003))
                {
                    bInitSuccess = true;
                    break;
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }

            ASSERT_TRUE(bInitSuccess) << "Failed to initialize the UDP socket";

            // Test constants
            const uint16_t unTestDataId = 1200;
            const std::string szIpAddr  = "127.0.0.1";
            const int nPort             = 11003;

            // ======================== UINT8_T TESTS ========================
            {
                // Small packet (1 element)
                rovecomm::RoveCommPacket<uint8_t> stSmallPacket{.unDataId    = unTestDataId,
                                                                .unDataCount = 1,
                                                                .eDataType   = manifest::DataTypes::UINT8_T,
                                                                .vData       = {UINT8_MAX}};

                // Medium packet (10 elements)
                rovecomm::RoveCommPacket<uint8_t> stMediumPacket{.unDataId = unTestDataId, .unDataCount = 10, .eDataType = manifest::DataTypes::UINT8_T, .vData = {}};
                for (uint8_t ucI = 0; ucI < 10; ucI++)
                {
                    stMediumPacket.vData.push_back(ucI * 25);    // Values 0, 25, 50, 75, 100, 125, 150, 175, 200, 225
                }

                // Large packet (100 elements)
                rovecomm::RoveCommPacket<uint8_t> stLargePacket{.unDataId = unTestDataId, .unDataCount = 100, .eDataType = manifest::DataTypes::UINT8_T, .vData = {}};
                for (int nI = 0; nI < 100; nI++)
                {
                    stLargePacket.vData.push_back(nI % 256);
                }

                // Send and verify
                ssize_t siBytesSent = RoveCommUDPNode.Send<uint8_t>(stSmallPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 7) << "Failed to send small uint8_t packet";

                siBytesSent = RoveCommUDPNode.Send<uint8_t>(stMediumPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 16) << "Failed to send medium uint8_t packet";

                siBytesSent = RoveCommUDPNode.Send<uint8_t>(stLargePacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 106) << "Failed to send large uint8_t packet";
            }

            // ======================== UINT16_T TESTS ========================
            {
                // Small packet (1 element)
                rovecomm::RoveCommPacket<uint16_t> stSmallPacket{.unDataId    = unTestDataId,
                                                                 .unDataCount = 1,
                                                                 .eDataType   = manifest::DataTypes::UINT16_T,
                                                                 .vData       = {UINT16_MAX}};

                // Medium packet (10 elements)
                rovecomm::RoveCommPacket<uint16_t> stMediumPacket{.unDataId = unTestDataId, .unDataCount = 10, .eDataType = manifest::DataTypes::UINT16_T, .vData = {}};
                for (uint16_t usI = 0; usI < 10; usI++)
                {
                    stMediumPacket.vData.push_back(usI * 6000);
                }

                // Large packet (50 elements)
                rovecomm::RoveCommPacket<uint16_t> stLargePacket{.unDataId = unTestDataId, .unDataCount = 50, .eDataType = manifest::DataTypes::UINT16_T, .vData = {}};
                for (int nI = 0; nI < 50; nI++)
                {
                    stLargePacket.vData.push_back(nI * 1000);
                }

                // Send and verify
                ssize_t siBytesSent = RoveCommUDPNode.Send<uint16_t>(stSmallPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 8) << "Failed to send small uint16_t packet";

                siBytesSent = RoveCommUDPNode.Send<uint16_t>(stMediumPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 26) << "Failed to send medium uint16_t packet";

                siBytesSent = RoveCommUDPNode.Send<uint16_t>(stLargePacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 106) << "Failed to send large uint16_t packet";
            }

            // ======================== UINT32_T TESTS ========================
            {
                // Small packet (1 element)
                rovecomm::RoveCommPacket<uint32_t> stSmallPacket{
                    .unDataId    = unTestDataId,
                    .unDataCount = 1,
                    .eDataType   = manifest::DataTypes::UINT32_T,
                    .vData       = {UINT32_MAX},
                };

                // Medium packet (10 elements)
                rovecomm::RoveCommPacket<uint32_t> stMediumPacket{.unDataId = unTestDataId, .unDataCount = 10, .eDataType = manifest::DataTypes::UINT32_T, .vData = {}};
                for (uint32_t unI = 0; unI < 10; unI++)
                {
                    stMediumPacket.vData.push_back(unI * 400000000);
                }

                // Send and verify
                ssize_t siBytesSent = RoveCommUDPNode.Send<uint32_t>(stSmallPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 10) << "Failed to send small uint32_t packet";

                siBytesSent = RoveCommUDPNode.Send<uint32_t>(stMediumPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 46) << "Failed to send medium uint32_t packet";
            }

            // ======================== INT8_T TESTS ========================
            {
                // Small packet (1 element)
                rovecomm::RoveCommPacket<int8_t> stSmallPacket{
                    .unDataId    = unTestDataId,
                    .unDataCount = 1,
                    .eDataType   = manifest::DataTypes::INT8_T,
                    .vData       = {INT8_MIN},
                };

                // Medium packet (10 elements) with positive and negative values
                rovecomm::RoveCommPacket<int8_t> stMediumPacket{.unDataId = unTestDataId, .unDataCount = 10, .eDataType = manifest::DataTypes::INT8_T, .vData = {}};
                for (int nI = 0; nI < 10; nI++)
                {
                    stMediumPacket.vData.push_back(nI < 5 ? -nI * 20 : nI * 10);
                }

                // Send and verify
                ssize_t siBytesSent = RoveCommUDPNode.Send<int8_t>(stSmallPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 7) << "Failed to send small int8_t packet";

                siBytesSent = RoveCommUDPNode.Send<int8_t>(stMediumPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 16) << "Failed to send medium int8_t packet";
            }

            // ======================== INT16_T TESTS ========================
            {
                // Small packet (1 element)
                rovecomm::RoveCommPacket<int16_t> stSmallPacket{.unDataId    = unTestDataId,
                                                                .unDataCount = 1,
                                                                .eDataType   = manifest::DataTypes::INT16_T,
                                                                .vData       = {INT16_MIN}};

                // Medium packet (10 elements)
                rovecomm::RoveCommPacket<int16_t> stMediumPacket{.unDataId = unTestDataId, .unDataCount = 10, .eDataType = manifest::DataTypes::INT16_T, .vData = {}};
                for (int nI = 0; nI < 10; nI++)
                {
                    stMediumPacket.vData.push_back(nI < 5 ? -nI * 5000 : nI * 3000);
                }

                // Send and verify
                ssize_t siBytesSent = RoveCommUDPNode.Send<int16_t>(stSmallPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 8) << "Failed to send small int16_t packet";

                siBytesSent = RoveCommUDPNode.Send<int16_t>(stMediumPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 26) << "Failed to send medium int16_t packet";
            }

            // ======================== INT32_T TESTS ========================
            {
                // Small packet (1 element)
                rovecomm::RoveCommPacket<int32_t> stSmallPacket{.unDataId    = unTestDataId,
                                                                .unDataCount = 1,
                                                                .eDataType   = manifest::DataTypes::INT32_T,
                                                                .vData       = {INT32_MIN}};

                // Medium packet (10 elements)
                rovecomm::RoveCommPacket<int32_t> stMediumPacket{.unDataId = unTestDataId, .unDataCount = 10, .eDataType = manifest::DataTypes::INT32_T, .vData = {}};
                for (int nI = 0; nI < 10; nI++)
                {
                    stMediumPacket.vData.push_back(nI < 5 ? -nI * 400000000 : nI * 300000000);
                }

                // Send and verify
                ssize_t siBytesSent = RoveCommUDPNode.Send<int32_t>(stSmallPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 10) << "Failed to send small int32_t packet";

                siBytesSent = RoveCommUDPNode.Send<int32_t>(stMediumPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 46) << "Failed to send medium int32_t packet";
            }

            // ======================== FLOAT TESTS ========================
            {
                // Small packet (1 element)
                rovecomm::RoveCommPacket<float> stSmallPacket{.unDataId = unTestDataId, .unDataCount = 1, .eDataType = manifest::DataTypes::FLOAT_T, .vData = {3.14159f}};

                // Medium packet (10 elements)
                rovecomm::RoveCommPacket<float> stMediumPacket{.unDataId = unTestDataId, .unDataCount = 10, .eDataType = manifest::DataTypes::FLOAT_T, .vData = {}};
                for (int nI = 0; nI < 10; nI++)
                {
                    stMediumPacket.vData.push_back(nI * 0.5f);
                }

                // Send and verify
                ssize_t siBytesSent = RoveCommUDPNode.Send<float>(stSmallPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 10) << "Failed to send small float packet";

                siBytesSent = RoveCommUDPNode.Send<float>(stMediumPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 46) << "Failed to send medium float packet";
            }

            // ======================== DOUBLE TESTS ========================
            {
                // Small packet (1 element)
                rovecomm::RoveCommPacket<double> stSmallPacket{.unDataId    = unTestDataId,
                                                               .unDataCount = 1,
                                                               .eDataType   = manifest::DataTypes::DOUBLE_T,
                                                               .vData       = {3.14159265358979323846}};

                // Medium packet (10 elements)
                rovecomm::RoveCommPacket<double> stMediumPacket{.unDataId = unTestDataId, .unDataCount = 10, .eDataType = manifest::DataTypes::DOUBLE_T, .vData = {}};
                for (int nI = 0; nI < 10; nI++)
                {
                    stMediumPacket.vData.push_back(nI * 0.333333333333333);
                }

                // Send and verify
                ssize_t siBytesSent = RoveCommUDPNode.Send<double>(stSmallPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 14) << "Failed to send small double packet";

                siBytesSent = RoveCommUDPNode.Send<double>(stMediumPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 86) << "Failed to send medium double packet";
            }

            // ======================== CHAR TESTS ========================
            {
                // Small packet (1 element)
                rovecomm::RoveCommPacket<char> stSmallPacket{
                    .unDataId    = unTestDataId,
                    .unDataCount = 1,
                    .eDataType   = manifest::DataTypes::CHAR,
                    .vData       = {'X'},
                };

                // Medium packet - string "Hello World!"
                std::string szMessage = "Hello World!";
                rovecomm::RoveCommPacket<char> stMediumPacket{
                    .unDataId    = unTestDataId,
                    .unDataCount = static_cast<uint16_t>(szMessage.size()),
                    .eDataType   = manifest::DataTypes::CHAR,
                    .vData       = {szMessage.begin(), szMessage.end()},
                };

                // Large packet - Lorem ipsum
                std::string szLoremIpsum = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
                rovecomm::RoveCommPacket<char> stLargePacket{
                    .unDataId    = unTestDataId,
                    .unDataCount = static_cast<uint16_t>(szLoremIpsum.size()),
                    .eDataType   = manifest::DataTypes::CHAR,
                    .vData       = {szLoremIpsum.begin(), szLoremIpsum.end()},
                };

                // Send and verify
                ssize_t siBytesSent = RoveCommUDPNode.Send<char>(stSmallPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, 7) << "Failed to send small char packet";

                siBytesSent = RoveCommUDPNode.Send<char>(stMediumPacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, szMessage.size() + 6) << "Failed to send medium char packet";

                siBytesSent = RoveCommUDPNode.Send<char>(stLargePacket, szIpAddr, nPort);
                EXPECT_EQ(siBytesSent, szLoremIpsum.size() + 6) << "Failed to send large char packet";
            }

            // Close the socket
            RoveCommUDPNode.Close();
        },
        3,         // 3 total attempts
        60000);    // 60 second timeout (60,000 ms) - increased for multiple packet tests
}

/******************************************************************************
 * @brief Main function for running the tests.
 *
 *
 * @author clayjay3 (claytonraycowen@gmail.com)
 * @date 2025-04-21
 ******************************************************************************/
TEST(RoveCommUDP, CallbackMultipleDataTypes)
{
    // Run the test via the RunTimedTest function to allow for retries and timeouts.
    testutils::RunTimedTest(
        []()
        {
            // Create RoveComm Nodes
            rovecomm::RoveCommUDP RoveCommUDPNode;

            // Flag to check if the initialization was successful
            bool bInitSuccess = false;

            // Give the node three chances to initialize the socket
            for (int nI = 0; nI < 3; ++nI)
            {
                if (RoveCommUDPNode.Init(11004))
                {
                    bInitSuccess = true;
                    break;
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }

            ASSERT_TRUE(bInitSuccess) << "Failed to initialize the UDP socket";

            // Setup dummy address for callbacks
            struct sockaddr_in saUDPClientAddr;
            memset(&saUDPClientAddr, 0, sizeof(saUDPClientAddr));
            saUDPClientAddr.sin_family = AF_INET;
            saUDPClientAddr.sin_port   = htons(11004);
            inet_pton(AF_INET, "127.0.0.1", &saUDPClientAddr.sin_addr);

            // Constants for test
            const uint16_t unTestDataId = 1300;

            // Test flags for each data type
            bool bUint8CallbackInvoked  = false;
            bool bUint16CallbackInvoked = false;
            bool bUint32CallbackInvoked = false;
            bool bInt8CallbackInvoked   = false;
            bool bInt16CallbackInvoked  = false;
            bool bInt32CallbackInvoked  = false;
            bool bFloatCallbackInvoked  = false;
            bool bDoubleCallbackInvoked = false;
            bool bCharCallbackInvoked   = false;

            // ======================== UINT8_T CALLBACK ========================
            {
                // Test data
                std::vector<uint8_t> vExpectedData = {1, 2, 3, 255};

                // Add callback
                RoveCommUDPNode.On<uint8_t>(unTestDataId,
                                            [&](const rovecomm::RoveCommPacket<uint8_t>& stPacket)
                                            {
                                                bUint8CallbackInvoked = true;
                                                EXPECT_EQ(stPacket.unDataId, unTestDataId);
                                                EXPECT_EQ(stPacket.unDataCount, vExpectedData.size());
                                                EXPECT_EQ(stPacket.eDataType, manifest::DataTypes::UINT8_T);

                                                for (size_t siI = 0; siI < stPacket.vData.size(); siI++)
                                                {
                                                    EXPECT_EQ(stPacket.vData[siI], vExpectedData[siI]);
                                                }
                                            });

                // Create packet and test callback
                rovecomm::RoveCommPacket<uint8_t> stPacket{.unDataId    = unTestDataId,
                                                           .unDataCount = static_cast<uint16_t>(vExpectedData.size()),
                                                           .eDataType   = manifest::DataTypes::UINT8_T,
                                                           .vData       = vExpectedData};

                // Pack the packet
                std::vector<uint8_t> vData = rovecomm::PackPacket(stPacket);

                // Process the packet
                RoveCommUDPNode.ProcessPacket<uint8_t>(vData, saUDPClientAddr);
            }

            // ======================== UINT16_T CALLBACK ========================
            {
                // Test data
                std::vector<uint16_t> vExpectedData = {1000, 2000, 3000, 65535};

                // Add callback
                RoveCommUDPNode.On<uint16_t>(unTestDataId,
                                             [&](const rovecomm::RoveCommPacket<uint16_t>& stPacket)
                                             {
                                                 bUint16CallbackInvoked = true;
                                                 EXPECT_EQ(stPacket.unDataId, unTestDataId);
                                                 EXPECT_EQ(stPacket.unDataCount, vExpectedData.size());
                                                 EXPECT_EQ(stPacket.eDataType, manifest::DataTypes::UINT16_T);

                                                 for (size_t siI = 0; siI < stPacket.vData.size(); siI++)
                                                 {
                                                     EXPECT_EQ(stPacket.vData[siI], vExpectedData[siI]);
                                                 }
                                             });

                // Create packet and test callback
                rovecomm::RoveCommPacket<uint16_t> stPacket{.unDataId    = unTestDataId,
                                                            .unDataCount = static_cast<uint16_t>(vExpectedData.size()),
                                                            .eDataType   = manifest::DataTypes::UINT16_T,
                                                            .vData       = vExpectedData};

                // Pack the packet
                std::vector<uint8_t> vData = rovecomm::PackPacket(stPacket);

                // Process the packet
                RoveCommUDPNode.ProcessPacket<uint16_t>(vData, saUDPClientAddr);
            }

            // ======================== INT32_T CALLBACK ========================
            {
                // Test data
                std::vector<int32_t> vExpectedData = {-100000, -1, 0, 1, 100000};

                // Add callback
                RoveCommUDPNode.On<int32_t>(unTestDataId,
                                            [&](const rovecomm::RoveCommPacket<int32_t>& stPacket)
                                            {
                                                bInt32CallbackInvoked = true;
                                                EXPECT_EQ(stPacket.unDataId, unTestDataId);
                                                EXPECT_EQ(stPacket.unDataCount, vExpectedData.size());
                                                EXPECT_EQ(stPacket.eDataType, manifest::DataTypes::INT32_T);

                                                for (size_t siI = 0; siI < stPacket.vData.size(); siI++)
                                                {
                                                    EXPECT_EQ(stPacket.vData[siI], vExpectedData[siI]);
                                                }
                                            });

                // Create packet and test callback
                rovecomm::RoveCommPacket<int32_t> stPacket{.unDataId    = unTestDataId,
                                                           .unDataCount = static_cast<uint16_t>(vExpectedData.size()),
                                                           .eDataType   = manifest::DataTypes::INT32_T,
                                                           .vData       = vExpectedData};

                // Pack the packet
                std::vector<uint8_t> vData = rovecomm::PackPacket(stPacket);

                // Process the packet
                RoveCommUDPNode.ProcessPacket<int32_t>(vData, saUDPClientAddr);
            }

            // ======================== FLOAT CALLBACK ========================
            {
                // Test data
                std::vector<float> vExpectedData = {-3.14f, 0.0f, 2.718f, 1000.5f};

                // Add callback
                RoveCommUDPNode.On<float>(unTestDataId,
                                          [&](const rovecomm::RoveCommPacket<float>& stPacket)
                                          {
                                              bFloatCallbackInvoked = true;
                                              EXPECT_EQ(stPacket.unDataId, unTestDataId);
                                              EXPECT_EQ(stPacket.unDataCount, vExpectedData.size());
                                              EXPECT_EQ(stPacket.eDataType, manifest::DataTypes::FLOAT_T);

                                              for (size_t siI = 0; siI < stPacket.vData.size(); siI++)
                                              {
                                                  EXPECT_FLOAT_EQ(stPacket.vData[siI], vExpectedData[siI]);
                                              }
                                          });

                // Create packet and test callback
                rovecomm::RoveCommPacket<float> stPacket{.unDataId    = unTestDataId,
                                                         .unDataCount = static_cast<uint16_t>(vExpectedData.size()),
                                                         .eDataType   = manifest::DataTypes::FLOAT_T,
                                                         .vData       = vExpectedData};

                // Pack the packet
                std::vector<uint8_t> vData = rovecomm::PackPacket(stPacket);

                // Process the packet
                RoveCommUDPNode.ProcessPacket<float>(vData, saUDPClientAddr);
            }

            // ======================== DOUBLE CALLBACK ========================
            {
                // Test data
                std::vector<double> vExpectedData = {-3.14, 0.0, 2.718, 1000.5};

                // Add callback
                RoveCommUDPNode.On<double>(unTestDataId,
                                           [&](const rovecomm::RoveCommPacket<double>& stPacket)
                                           {
                                               bDoubleCallbackInvoked = true;
                                               EXPECT_EQ(stPacket.unDataId, unTestDataId);
                                               EXPECT_EQ(stPacket.unDataCount, vExpectedData.size());
                                               EXPECT_EQ(stPacket.eDataType, manifest::DataTypes::DOUBLE_T);

                                               for (size_t siI = 0; siI < stPacket.vData.size(); siI++)
                                               {
                                                   EXPECT_DOUBLE_EQ(stPacket.vData[siI], vExpectedData[siI]);
                                               }
                                           });

                // Create packet and test callback
                rovecomm::RoveCommPacket<double> stPacket{.unDataId    = unTestDataId,
                                                          .unDataCount = static_cast<uint16_t>(vExpectedData.size()),
                                                          .eDataType   = manifest::DataTypes::DOUBLE_T,
                                                          .vData       = vExpectedData};

                // Pack the packet
                std::vector<uint8_t> vData = rovecomm::PackPacket(stPacket);

                // Process the packet
                RoveCommUDPNode.ProcessPacket<double>(vData, saUDPClientAddr);
            }

            // ======================== CHAR CALLBACK ========================
            {
                // Test data - "Hello"
                std::string szMessage = "Hello";
                std::vector<char> vExpectedData(szMessage.begin(), szMessage.end());

                // Add callback
                RoveCommUDPNode.On<char>(unTestDataId,
                                         [&](const rovecomm::RoveCommPacket<char>& stPacket)
                                         {
                                             bCharCallbackInvoked = true;
                                             EXPECT_EQ(stPacket.unDataId, unTestDataId);
                                             EXPECT_EQ(stPacket.unDataCount, vExpectedData.size());
                                             EXPECT_EQ(stPacket.eDataType, manifest::DataTypes::CHAR);

                                             for (size_t siI = 0; siI < stPacket.vData.size(); siI++)
                                             {
                                                 EXPECT_EQ(stPacket.vData[siI], vExpectedData[siI]);
                                             }
                                         });

                // Create packet and test callback
                rovecomm::RoveCommPacket<char> stPacket{.unDataId    = unTestDataId,
                                                        .unDataCount = static_cast<uint16_t>(vExpectedData.size()),
                                                        .eDataType   = manifest::DataTypes::CHAR,
                                                        .vData       = vExpectedData};

                // Pack the packet
                std::vector<uint8_t> vData = rovecomm::PackPacket(stPacket);

                // Process the packet
                RoveCommUDPNode.ProcessPacket<char>(vData, saUDPClientAddr);
            }

            // Check if all the callbacks were invoked
            EXPECT_TRUE(bUint8CallbackInvoked) << "uint8_t callback was not invoked";
            EXPECT_TRUE(bUint16CallbackInvoked) << "uint16_t callback was not invoked";
            EXPECT_TRUE(bInt32CallbackInvoked) << "int32_t callback was not invoked";
            EXPECT_TRUE(bFloatCallbackInvoked) << "float callback was not invoked";
            EXPECT_TRUE(bCharCallbackInvoked) << "char callback was not invoked";

            // Close the socket
            RoveCommUDPNode.Close();
        },
        3,         // 3 total attempts
        30000);    // 30 second timeout (30,000 ms)
}

/******************************************************************************
 * @brief Main function for running the tests.
 *
 *
 * @author clayjay3 (claytonraycowen@gmail.com)
 * @date 2025-04-21
 ******************************************************************************/
TEST(RoveCommUDP, ManifestIntegration)
{
    // Run the test via the RunTimedTest function to allow for retries and timeouts.
    testutils::RunTimedTest(
        []()
        {
            // Create RoveComm Nodes
            rovecomm::RoveCommUDP RoveCommUDPNode;

            // Flag to check if the initialization was successful
            bool bInitSuccess = false;

            // Give the node three chances to initialize the socket
            for (int nI = 0; nI < 3; ++nI)
            {
                if (RoveCommUDPNode.Init(11004))
                {
                    bInitSuccess = true;
                    break;
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }

            ASSERT_TRUE(bInitSuccess) << "Failed to initialize the UDP socket";

            // Setup dummy address for callbacks
            struct sockaddr_in saUDPClientAddr;
            memset(&saUDPClientAddr, 0, sizeof(saUDPClientAddr));
            saUDPClientAddr.sin_family = AF_INET;
            saUDPClientAddr.sin_port   = htons(11004);
            inet_pton(AF_INET, "127.0.0.1", &saUDPClientAddr.sin_addr);

            {
                constexpr manifest::ManifestEntry stTestEntry{
                    .DATA_ID    = 1000,
                    .DATA_COUNT = 3,
                    .DATA_TYPE  = manifest::DataTypes::INT32_T,
                };

                ssize_t siBytesSent = RoveCommUDPNode.Send<stTestEntry>({-100, 0, 100}, "127.0.0.1", 11004);
                EXPECT_EQ(siBytesSent, stTestEntry.DATA_COUNT * sizeof(int32_t) + rovecomm::ROVECOMM_PACKET_HEADER_SIZE)
                    << "Failed to send int32_t packet using manifest entry";
            }

            {
                constexpr manifest::ManifestEntry stTestEntry{
                    .DATA_ID    = 1001,
                    .DATA_COUNT = 256,
                    .DATA_TYPE  = manifest::DataTypes::CHAR,
                };

                ssize_t siBytesSent = RoveCommUDPNode.Send<stTestEntry>("TEST MESSAGE", "127.0.0.1", 11004);
                EXPECT_EQ(siBytesSent, 256 + rovecomm::ROVECOMM_PACKET_HEADER_SIZE) << "Failed to send small char packet using manifest entry";
            }

            // Close the socket
            RoveCommUDPNode.Close();
        },
        3,         // 3 total attempts
        30000);    // 30 second timeout (30,000 ms)
}
