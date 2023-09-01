/******************************************************************************
 * @brief
 *
 * @file Packet.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-08-30
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef PACKET_H
#define PACKET_H

#include "Consts.h"

#include "Manifest.h"
using namespace manifest;

struct DataPacket
{
    public:
        uint16_t unDataId;
        uint16_t unDataCount;
        uint8_t unDataType;
        char cData[constants::ROVECOMM_PACKET_MAX_DATA_COUNT];

        AddressEntry stIP;
};

enum DataType_T
{
    INT8_T   = 0,
    UINT8_T  = 1,
    INT16_T  = 2,
    UINT16_T = 3,
    INT32_T  = 4,
    UINT32_T = 5,
    FLOAT    = 6,
    DOUBLE   = 7,
    CHAR     = 8
};

struct BytePacket
{
    public:
        uint8_t unBytes[constants::ROVECOMM_PACKET_HEADER_SIZE + sizeof(uint8_t) * constants::ROVECOMM_PACKET_MAX_DATA_COUNT];
};

struct BytePacket PackPacket(const uint16_t unDataId, const uint16_t unDataCount, const DataType_T stDataType, const void* pData);

struct DataPacket UnPackPacket(uint8_t BytePacket[]);

#endif    // PACKET_H
