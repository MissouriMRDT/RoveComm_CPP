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

/// \cond
#include <iostream>

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

        // The first byte of the data is the version number
        stData.unBytes[0] = ROVECOMM_VERSION;

        // The next two bytes are the data ID
        stData.unBytes[1] = stPacket.unDataId >> 8;
        stData.unBytes[2] = stPacket.unDataId;

        // The next two bytes are the data count
        stData.unBytes[3] = stPacket.unDataCount >> 8;
        stData.unBytes[4] = stPacket.unDataCount;

        // The next byte is the data type
        stData.unBytes[5] = static_cast<uint8_t>(stPacket.eDataType);

        // The rest of the data is the data payload
        size_t siDataSize = sizeof(T) * stPacket.unDataCount;
        // Initialize pointers for looping through data.
        uint8_t* pDataPtr    = &stData.unBytes[ROVECOMM_PACKET_HEADER_SIZE];
        const T* pPacketData = stPacket.vData.data();
        // Loop through data.
        for (uint16_t unInter = 0; unInter < stPacket.unDataCount; ++unInter)
        {
            // Check if data is float32.
            if constexpr (std::is_same_v<T, float>)
            {
                // Convert float to network order and add to RoveCommData.
                uint32_t unResult;
                memcpy(&unResult, &pPacketData[unInter], sizeof(float));
                unResult = htonl(unResult);
                // Add converted data to RoveCommData.
                memcpy(pDataPtr, &unResult, sizeof(uint32_t));
                pDataPtr += sizeof(uint32_t);
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                // Convert double to network order and add to RoveCommData.
                uint64_t unResult;
                memcpy(&unResult, &pPacketData[unInter], sizeof(double));
                unResult = htonll(unResult);
                // Add converted data to RoveCommData.
                memcpy(pDataPtr, &unResult, sizeof(uint64_t));
                pDataPtr += sizeof(uint64_t);
            }
            else
            {
                // For other types, just copy the data as is
                memcpy(pDataPtr, &pPacketData[unInter], sizeof(T));
                pDataPtr += sizeof(T);
            }
        }

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

        // Copy the data payload from stData to stPacket's vData vector
        stPacket.vData.resize(stPacket.unDataCount);

        // Initialize pointers for looping through data.
        const uint8_t* pDataPtr = &stData.unBytes[ROVECOMM_PACKET_HEADER_SIZE];
        T* pPacketData          = stPacket.vData.data();
        // Loop through data.
        for (uint16_t unIter = 0; unIter < stPacket.unDataCount; ++unIter)
        {
            // Check if data is float32.
            if constexpr (std::is_same_v<T, float>)
            {
                // Convert float from network order and add to RoveCommPacket.
                uint32_t unResult;
                memcpy(&unResult, pDataPtr + (unIter * sizeof(uint32_t)), sizeof(uint32_t));
                unResult = ntohl(unResult);
                float fFloatResult;
                memcpy(&fFloatResult, &unResult, sizeof(uint32_t));
                // Add converted data to RoveCommPacket.
                stPacket.vData[unIter] = fFloatResult;
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                // Convert double from network order and add to RoveCommPacket.
                uint64_t unResult;
                memcpy(&unResult, pDataPtr + (unIter * sizeof(uint64_t)), sizeof(uint64_t));
                unResult = ntohll(unResult);
                double fDoubleResult;
                memcpy(&fDoubleResult, &unResult, sizeof(uint64_t));
                // Add converted data to RoveCommPacket.
                stPacket.vData[unIter] = fDoubleResult;
            }
            else
            {
                // For other types, just copy the data as is
                T tValue;
                memcpy(&tValue, pDataPtr + (unIter * sizeof(T)), sizeof(T));
                // Add converted data to RoveCommPacket.
                stPacket.vData[unIter] = tValue;
            }
        }

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
