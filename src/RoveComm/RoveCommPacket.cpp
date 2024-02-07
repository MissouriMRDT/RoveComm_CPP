/******************************************************************************
 * @brief RoveComm Packet Implementation.
 *
 * @file RoveCommPacket.cpp
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-05
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#include "RoveCommPacket.h"
#include <iostream>

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
     * @brief Create a RoveCommData structure from a RoveCommPacket structure.
     *
     * @tparam T - The type of data that is to be sent or received. This can be
     *             any of the types defined in the manifest.
     * @param stPacket - The packet to be packed into a RoveCommData structure.
     * @return RoveCommData - The packed data.
     *
     * @note The accepted versions of the Pack Packet function are templated and
     *       are explicitly instantiated at the end of this file.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    template<typename T>
    RoveCommData PackPacket(const RoveCommPacket<T>& stPacket)
    {
        RoveCommData stData;

        stData.unBytes[0] = ROVECOMM_VERSION;
        stData.unBytes[1] = stPacket.unDataId >> 8;
        stData.unBytes[2] = stPacket.unDataId;
        stData.unBytes[3] = stPacket.unDataCount >> 8;
        stData.unBytes[4] = stPacket.unDataCount;
        stData.unBytes[5] = static_cast<uint8_t>(stPacket.eDataType);

        size_t siDataSize = sizeof(T) * stPacket.unDataCount;
        memcpy(&stData.unBytes[ROVECOMM_PACKET_HEADER_SIZE], stPacket.vData.data(), siDataSize);

        return stData;
    }

    // Explicit instantiation of the template for supported types
    template RoveCommData PackPacket<uint8_t>(const RoveCommPacket<uint8_t>& stPacket);
    template RoveCommData PackPacket<uint16_t>(const RoveCommPacket<uint16_t>& stPacket);
    template RoveCommData PackPacket<uint32_t>(const RoveCommPacket<uint32_t>& stPacket);
    template RoveCommData PackPacket<int8_t>(const RoveCommPacket<int8_t>& stPacket);
    template RoveCommData PackPacket<int16_t>(const RoveCommPacket<int16_t>& stPacket);
    template RoveCommData PackPacket<int32_t>(const RoveCommPacket<int32_t>& stPacket);
    template RoveCommData PackPacket<float>(const RoveCommPacket<float>& stPacket);
    template RoveCommData PackPacket<double>(const RoveCommPacket<double>& stPacket);
    template RoveCommData PackPacket<char>(const RoveCommPacket<char>& stPacket);

    /******************************************************************************
     * @brief Create a RoveCommPacket structure from a RoveCommData structure.
     *
     * @tparam T - The type of data that is to be sent or received. This can be
     *             any of the types defined in the manifest.
     * @param stData - The data to be unpacked into a RoveCommPacket structure.
     * @return RoveCommPacket<T> - The unpacked packet.
     *
     * @note The accepted versions of the Unpack Data function are templated and
     *       are explicitly instantiated at the end of this file.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    template<typename T>
    RoveCommPacket<T> UnpackData(const RoveCommData& stData)
    {
        RoveCommPacket<T> stPacket;

        // Extract data from stData and fill stPacket
        stPacket.unDataId    = (stData.unBytes[1] << 8) | stData.unBytes[2];
        stPacket.unDataCount = (stData.unBytes[3] << 8) | stData.unBytes[4];
        stPacket.eDataType   = static_cast<manifest::DataTypes>(stData.unBytes[5]);

        // Calculate the size of the data payload
        size_t siDataSize = sizeof(T) * stPacket.unDataCount;

        // Copy the data payload from stData to stPacket's vData vector
        stPacket.vData.resize(stPacket.unDataCount);
        memcpy(stPacket.vData.data(), &stData.unBytes[ROVECOMM_PACKET_HEADER_SIZE], siDataSize);

        return stPacket;
    }

    // Explicit instantiation of the template for supported types
    template RoveCommPacket<uint8_t> UnpackData<uint8_t>(const RoveCommData& stData);
    template RoveCommPacket<uint16_t> UnpackData<uint16_t>(const RoveCommData& stData);
    template RoveCommPacket<uint32_t> UnpackData<uint32_t>(const RoveCommData& stData);
    template RoveCommPacket<int8_t> UnpackData<int8_t>(const RoveCommData& stData);
    template RoveCommPacket<int16_t> UnpackData<int16_t>(const RoveCommData& stData);
    template RoveCommPacket<int32_t> UnpackData<int32_t>(const RoveCommData& stData);
    template RoveCommPacket<float> UnpackData<float>(const RoveCommData& stData);
    template RoveCommPacket<double> UnpackData<double>(const RoveCommData& stData);
    template RoveCommPacket<char> UnpackData<char>(const RoveCommData& stData);
}    // namespace rovecomm
