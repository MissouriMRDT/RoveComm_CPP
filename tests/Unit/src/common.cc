/******************************************************************************
 * @brief Unit test for utilities and manifest helper functions.
 *
 * @file common.cc
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-07
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#include "../../../src/RoveComm/RoveComm.h"
#include "../../TestUtils.h"

/// \cond

#include <gtest/gtest.h>

/// \endcond

/******************************************************************************
 * @brief Main function for running the tests.
 *
 *
 * @author clayjay3 (claytonraycowen@gmail.com)
 * @date 2025-04-21
 ******************************************************************************/
TEST(RoveCommUtils, PacketUtils)
{
    // Test packet creation utilities
    constexpr manifest::ManifestEntry stTestEntry{
        .DATA_ID    = 1000,
        .DATA_COUNT = 3,
        .DATA_TYPE  = manifest::DataTypes::INT32_T,
    };

    // Test packet creation utilities

    rovecomm::RoveCommPacket<int32_t> stPacket;

    stPacket = rovecomm::CreatePacket<stTestEntry>();
    EXPECT_EQ(stPacket.unDataId, stTestEntry.DATA_ID);
    EXPECT_EQ(stPacket.unDataCount, stTestEntry.DATA_COUNT);
    EXPECT_EQ(stPacket.eDataType, stTestEntry.DATA_TYPE);
    EXPECT_EQ(stPacket.vData.size(), stTestEntry.DATA_COUNT);

    stPacket = rovecomm::CreatePacket<stTestEntry>(1, 2, 3);
    EXPECT_EQ(stPacket.unDataId, stTestEntry.DATA_ID);
    EXPECT_EQ(stPacket.unDataCount, stTestEntry.DATA_COUNT);
    EXPECT_EQ(stPacket.eDataType, stTestEntry.DATA_TYPE);
    EXPECT_EQ(stPacket.vData[0], 1);
    EXPECT_EQ(stPacket.vData[1], 2);
    EXPECT_EQ(stPacket.vData[2], 3);

    stPacket = rovecomm::CreatePacket<stTestEntry>({1, 2, 3});
    EXPECT_EQ(stPacket.unDataId, stTestEntry.DATA_ID);
    EXPECT_EQ(stPacket.unDataCount, stTestEntry.DATA_COUNT);
    EXPECT_EQ(stPacket.eDataType, stTestEntry.DATA_TYPE);
    EXPECT_EQ(stPacket.vData[0], 1);
    EXPECT_EQ(stPacket.vData[1], 2);
    EXPECT_EQ(stPacket.vData[2], 3);

    std::array<int32_t, stTestEntry.DATA_COUNT> aData = {1, 2, 3};
    stPacket                                          = rovecomm::CreatePacket<stTestEntry>(aData);
    EXPECT_EQ(stPacket.unDataId, stTestEntry.DATA_ID);
    EXPECT_EQ(stPacket.unDataCount, stTestEntry.DATA_COUNT);
    EXPECT_EQ(stPacket.eDataType, stTestEntry.DATA_TYPE);
    EXPECT_EQ(stPacket.vData[0], 1);
    EXPECT_EQ(stPacket.vData[1], 2);
    EXPECT_EQ(stPacket.vData[2], 3);

    std::string szData                                = "Hello, World";
    constexpr manifest::ManifestEntry stCharTestEntry = {
        .DATA_ID    = 2000,
        .DATA_COUNT = 256,
        .DATA_TYPE  = manifest::DataTypes::CHAR,
    };

    rovecomm::RoveCommPacket<char> stCharPacket = rovecomm::CreatePacket<stCharTestEntry>(szData);
    EXPECT_EQ(stCharPacket.unDataId, stCharTestEntry.DATA_ID);
    EXPECT_EQ(stCharPacket.unDataCount, stCharTestEntry.DATA_COUNT);
    EXPECT_EQ(stCharPacket.eDataType, stCharTestEntry.DATA_TYPE);
    // Data should be the string followed by null characters to fill the rest of the packet
    EXPECT_EQ(stCharPacket.vData.size(), stCharTestEntry.DATA_COUNT);
    for (size_t siI = 0; siI < szData.size(); siI++)
    {
        EXPECT_EQ(stCharPacket.vData[siI], szData[siI]);
    }
}

/******************************************************************************
 * @brief Main function for running the tests.
 *
 *
 * @author clayjay3 (claytonraycowen@gmail.com)
 * @date 2025-04-21
 ******************************************************************************/
TEST(RoveCommUtils, ManifestHelpers)
{    // Test manifest helper functions

    // Find a board that has commands, telemetry, and errors for testing
    auto itValidEntry = std::find_if(manifest::BOARDS.begin(),
                                     manifest::BOARDS.end(),
                                     [](const auto& stBoard) { return !stBoard.COMMANDS.empty() && !stBoard.TELEMETRY.empty() && !stBoard.ERRORS.empty(); });
    ASSERT_NE(itValidEntry, manifest::BOARDS.end()) << "No valid board entry found in manifest for testing.";

    const auto& stBoard                  = *itValidEntry;
    const auto& stCommand                = stBoard.COMMANDS.front();
    const auto& stTelemetry              = stBoard.TELEMETRY.front();
    const auto& stError                  = stBoard.ERRORS.front();

    const manifest::BoardEntry& stResult = manifest::Helpers::FindBoardById(stCommand.DATA_ID);
    EXPECT_EQ(stResult.ADDRESS.IP_STR(), stBoard.ADDRESS.IP_STR());

    const uint16_t unInvalidBoardId = static_cast<int>(manifest::BOARDS.back().BOARD_ID) + 1;
    EXPECT_THROW(manifest::Helpers::FindBoardById(unInvalidBoardId), std::invalid_argument);

    const manifest::ManifestEntry& stCommandResult = manifest::Helpers::FindEntryById(stCommand.DATA_ID);
    EXPECT_EQ(stCommandResult.DATA_ID, stCommand.DATA_ID);
    EXPECT_EQ(stCommandResult.DATA_COUNT, stCommand.DATA_COUNT);
    EXPECT_EQ(stCommandResult.DATA_TYPE, stCommand.DATA_TYPE);

    const manifest::ManifestEntry& stTelemetryResult = manifest::Helpers::FindEntryById(stTelemetry.DATA_ID);
    EXPECT_EQ(stTelemetryResult.DATA_ID, stTelemetry.DATA_ID);
    EXPECT_EQ(stTelemetryResult.DATA_COUNT, stTelemetry.DATA_COUNT);
    EXPECT_EQ(stTelemetryResult.DATA_TYPE, stTelemetry.DATA_TYPE);

    const manifest::ManifestEntry& stErrorResult = manifest::Helpers::FindEntryById(stError.DATA_ID);
    EXPECT_EQ(stErrorResult.DATA_ID, stError.DATA_ID);
    EXPECT_EQ(stErrorResult.DATA_COUNT, stError.DATA_COUNT);
    EXPECT_EQ(stErrorResult.DATA_TYPE, stError.DATA_TYPE);

    uint16_t unInvalidDataId = stBoard.COMMANDS.back().DATA_ID + 1;
    EXPECT_THROW(manifest::Helpers::FindEntryById(unInvalidDataId), std::invalid_argument);

    // Test conversion between AddressEntry and string
    const manifest::AddressEntry stAddressFromString{"192.168.1.100"};
    const manifest::AddressEntry stAddress{192, 168, 1, 100};
    EXPECT_EQ(stAddressFromString.FIRST_OCTET, 192);
    EXPECT_EQ(stAddressFromString.SECOND_OCTET, 168);
    EXPECT_EQ(stAddressFromString.THIRD_OCTET, 1);
    EXPECT_EQ(stAddressFromString.FOURTH_OCTET, 100);
    EXPECT_EQ(stAddressFromString.IP_STR(), "192.168.1.100");
    EXPECT_EQ(stAddressFromString, stAddress);
}
