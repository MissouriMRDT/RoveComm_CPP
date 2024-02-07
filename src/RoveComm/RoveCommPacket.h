/******************************************************************************
 * @brief RoveComm Packet Definition.
 *
 * @file RoveCommPacket.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-05
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_PACKET_H
#define ROVECOMM_PACKET_H

#include "./RoveCommConsts.h"
#include "./RoveCommManifest.h"

#include <cstdint>
#include <cstring>
#include <vector>

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
    template<typename T>
    struct RoveCommPacket
    {
        public:
            uint16_t unDataId;
            uint16_t unDataCount;
            manifest::DataTypes eDataType;
            std::vector<T> vData;
    };

    struct RoveCommData
    {
        public:
            uint8_t unBytes[ROVECOMM_PACKET_HEADER_SIZE + sizeof(uint8_t) * ROVECOMM_PACKET_MAX_DATA_COUNT / 2];
    };

    template<typename T>
    RoveCommData PackPacket(const RoveCommPacket<T>& stPacket);

    template<typename T>
    RoveCommPacket<T> UnpackData(const RoveCommData& stData);
}    // namespace rovecomm

#endif    // ROVECOMM_PACKET_H
