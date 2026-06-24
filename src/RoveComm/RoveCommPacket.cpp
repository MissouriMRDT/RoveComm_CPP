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
#include <bit>

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
    std::vector<uint8_t> PackPacket(const RoveCommPacket<T>& stPacket)
    {
        size_t siPacketSize = ROVECOMM_PACKET_HEADER_SIZE + stPacket.GetDataSize();
        if (siPacketSize > ROVECOMM_PACKET_MAX_DATA_SIZE)
        {
            throw std::runtime_error("RoveComm Packet Exceeds Maximum Packet Size.");
        }
        std::vector<uint8_t> vData;
        vData.reserve(siPacketSize);

        // The first byte of the data is the version number
        vData.push_back(ROVECOMM_VERSION);

        // The next two bytes are the data ID
        vData.push_back(stPacket.unDataId >> 8);
        vData.push_back(stPacket.unDataId);

        // The next two bytes are the data count
        vData.push_back(stPacket.GetDataCount() >> 8);
        vData.push_back(stPacket.GetDataCount());

        // The next byte is the data type
        vData.push_back(stPacket.GetDataType());

        // The rest of the data is the data payload
        // Loop through data.
        for (T tEl : stPacket.vData)
        {
            // INT8_T, UINT8_T, CHAR
            if constexpr (manifest::Helpers::CToRoveCommType<T>::SIZE == 1)
            {
                // No need to convert to network order.
                vData.push_back(tEl);
            }
            // INT16_T, UINT16_T
            else if constexpr (manifest::Helpers::CToRoveCommType<T>::SIZE == 2)
            {
                // Convert to network order and add to RoveCommData.
                uint16_t unResult;
                memcpy(&unResult, &tEl, sizeof(unResult));
                vData.push_back(static_cast<uint8_t>(unResult >> (8 * 1)));
                vData.push_back(static_cast<uint8_t>(unResult >> (8 * 0)));
            }
            // INT32_T, UINT32_T, FLOAT_T
            else if constexpr (manifest::Helpers::CToRoveCommType<T>::SIZE == 4)
            {
                // Convert to network order and add to RoveCommData.
                uint32_t unResult;
                memcpy(&unResult, &tEl, sizeof(unResult));
                vData.push_back(static_cast<uint8_t>(unResult >> (8 * 3)));
                vData.push_back(static_cast<uint8_t>(unResult >> (8 * 2)));
                vData.push_back(static_cast<uint8_t>(unResult >> (8 * 1)));
                vData.push_back(static_cast<uint8_t>(unResult >> (8 * 0)));
            }
            // DOUBLE_T
            else if constexpr (manifest::Helpers::CToRoveCommType<T>::SIZE == 8)
            {
                // Convert to network order and add to RoveCommData.
                uint64_t unResult;
                memcpy(&unResult, &tEl, sizeof(unResult));
                vData.push_back(static_cast<uint8_t>(unResult >> (8 * 7)));
                vData.push_back(static_cast<uint8_t>(unResult >> (8 * 6)));
                vData.push_back(static_cast<uint8_t>(unResult >> (8 * 5)));
                vData.push_back(static_cast<uint8_t>(unResult >> (8 * 4)));
                vData.push_back(static_cast<uint8_t>(unResult >> (8 * 3)));
                vData.push_back(static_cast<uint8_t>(unResult >> (8 * 2)));
                vData.push_back(static_cast<uint8_t>(unResult >> (8 * 1)));
                vData.push_back(static_cast<uint8_t>(unResult >> (8 * 0)));
            }
        }

        return vData;
    }

    // Explicit instantiation of the template for supported types
    template std::vector<uint8_t> PackPacket<uint8_t>(const RoveCommPacket<uint8_t>& stPacket);
    template std::vector<uint8_t> PackPacket<uint16_t>(const RoveCommPacket<uint16_t>& stPacket);
    template std::vector<uint8_t> PackPacket<uint32_t>(const RoveCommPacket<uint32_t>& stPacket);
    template std::vector<uint8_t> PackPacket<int8_t>(const RoveCommPacket<int8_t>& stPacket);
    template std::vector<uint8_t> PackPacket<int16_t>(const RoveCommPacket<int16_t>& stPacket);
    template std::vector<uint8_t> PackPacket<int32_t>(const RoveCommPacket<int32_t>& stPacket);
    template std::vector<uint8_t> PackPacket<float>(const RoveCommPacket<float>& stPacket);
    template std::vector<uint8_t> PackPacket<double>(const RoveCommPacket<double>& stPacket);
    template std::vector<uint8_t> PackPacket<char>(const RoveCommPacket<char>& stPacket);

    /******************************************************************************
     * @brief Create a RoveCommPacket structure from a RoveCommData structure.
     *
     * @tparam T - The type of data that is to be sent or received. This can be
     *             any of the types defined in the manifest.
     * @param spBytes - The data to be unpacked into a RoveCommPacket structure.
     * @return RoveCommPacket<T> - The unpacked packet.
     *
     * @note The accepted versions of the Unpack Data function are templated and
     *       are explicitly instantiated at the end of this file.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    template<typename T>
    RoveCommPacket<T> UnpackData(std::span<const uint8_t> spBytes)
    {
        if (spBytes.size() < ROVECOMM_PACKET_HEADER_SIZE)
        {
            throw std::runtime_error("Not enough data to parse RoveCommPacket header.");
        }

        RoveCommPacket<T> stPacket;

        // Extract data from spBytes and fill stPacket
        stPacket.unDataId             = (spBytes[1] << 8) | spBytes[2];
        uint16_t unDataCount          = (spBytes[3] << 8) | spBytes[4];
        manifest::DataTypes eDataType = static_cast<manifest::DataTypes>(spBytes[5]);

        size_t siPacketSize           = ROVECOMM_PACKET_HEADER_SIZE + unDataCount * manifest::Helpers::DataTypeSize(eDataType);
        if (spBytes.size() < siPacketSize)
        {
            throw std::runtime_error("Packet header did not match packet size.");
        }
        if (manifest::Helpers::CToRoveCommType<T>::TYPE != eDataType)
        {
            throw std::runtime_error("Packet data type does not match.");
        }

        // Copy the data payload from spBytes to stPacket's vData vector
        stPacket.vData.reserve(unDataCount);

        // Loop through data.
        std::span<const uint8_t> spData = spBytes.subspan(ROVECOMM_PACKET_HEADER_SIZE);
        for (uint16_t unIt = 0; unIt < unDataCount * manifest::Helpers::DataTypeSize(eDataType);)
        {
            // INT8_T, UINT8_T, CHAR
            if constexpr (manifest::Helpers::CToRoveCommType<T>::SIZE == 1)
            {
                // No need to convert to host order.
                stPacket.vData.push_back(spData[unIt++]);
            }
            // INT16_T, UINT16_T
            else if constexpr (manifest::Helpers::CToRoveCommType<T>::SIZE == 2)
            {
                // Convert to host order and add to RoveCommData.
                T tResult;
                uint16_t unResult = spData[unIt++] << (8 * 1) |    //
                                    spData[unIt++] << (8 * 0);
                memcpy(&tResult, &unResult, sizeof(unResult));
                stPacket.vData.push_back(tResult);
            }
            // INT32_T, UINT32_T, FLOAT_T
            else if constexpr (manifest::Helpers::CToRoveCommType<T>::SIZE == 4)
            {
                // Convert to host order and add to RoveCommData.
                T tResult;
                uint32_t unResult = spData[unIt++] << (8 * 3) |    //
                                    spData[unIt++] << (8 * 2) |    //
                                    spData[unIt++] << (8 * 1) |    //
                                    spData[unIt++] << (8 * 0);
                memcpy(&tResult, &unResult, sizeof(unResult));
                stPacket.vData.push_back(tResult);
            }
            // DOUBLE_T
            else if constexpr (manifest::Helpers::CToRoveCommType<T>::SIZE == 8)
            {
                // Convert to host order and add to RoveCommData.
                T tResult;
                uint64_t unResult = static_cast<uint64_t>(spData[unIt++]) << (8 * 7) |    //
                                    static_cast<uint64_t>(spData[unIt++]) << (8 * 6) |    //
                                    static_cast<uint64_t>(spData[unIt++]) << (8 * 5) |    //
                                    static_cast<uint64_t>(spData[unIt++]) << (8 * 4) |    //
                                    static_cast<uint64_t>(spData[unIt++]) << (8 * 3) |    //
                                    static_cast<uint64_t>(spData[unIt++]) << (8 * 2) |    //
                                    static_cast<uint64_t>(spData[unIt++]) << (8 * 1) |    //
                                    static_cast<uint64_t>(spData[unIt++]) << (8 * 0);
                memcpy(&tResult, &unResult, sizeof(unResult));
                stPacket.vData.push_back(tResult);
            }
        }
        return stPacket;
    }

    // Explicit instantiation of the template for supported types
    template RoveCommPacket<uint8_t> UnpackData<uint8_t>(std::span<const uint8_t>);
    template RoveCommPacket<uint16_t> UnpackData<uint16_t>(std::span<const uint8_t>);
    template RoveCommPacket<uint32_t> UnpackData<uint32_t>(std::span<const uint8_t>);
    template RoveCommPacket<int8_t> UnpackData<int8_t>(std::span<const uint8_t>);
    template RoveCommPacket<int16_t> UnpackData<int16_t>(std::span<const uint8_t>);
    template RoveCommPacket<int32_t> UnpackData<int32_t>(std::span<const uint8_t>);
    template RoveCommPacket<float> UnpackData<float>(std::span<const uint8_t>);
    template RoveCommPacket<double> UnpackData<double>(std::span<const uint8_t>);
    template RoveCommPacket<char> UnpackData<char>(std::span<const uint8_t>);
}    // namespace rovecomm
