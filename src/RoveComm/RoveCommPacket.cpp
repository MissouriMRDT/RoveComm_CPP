/******************************************************************************
 * @brief RoveCommPacket Implementation for RoveComm C++ Library
 *
 * @file RoveCommPacket.cpp
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-10-08
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#include "RoveCommPacket.h"
#include "RoveCommConstants.h"
#include "RoveCommHelpers.h"

#include <cstring>
#include <iostream>
#include <memory>
#include <netinet/in.h>

size_t rovecomm::DataTypeSize(RoveCommDataType ucType)
{
    switch (ucType)
    {
        case INT8_T:
        case UINT8_T: return 1;
        case INT16_T:
        case UINT16_T: return 2;
        case INT32_T:
        case UINT32_T: return 4;
        case FLOAT_T: return 4;
        case DOUBLE_T: return 8;
        case CHAR: return 1;
        default:
        {
            // unknown data type
            return 0;
        }
    }
}

RoveCommPacketBuffer RoveCommPacket::Pack(const RoveCommPacket& packet)
{
    char* pDest = new char[packet.GetSize()];

    // Write header first
    pDest[0] = packet.GetVersionId();
    pDest[1] = packet.GetDataId() >> 8;    // convert to network order
    pDest[2] = packet.GetDataId() & 0xFF;
    pDest[3] = packet.GetDataCount() >> 8;
    pDest[4] = packet.GetDataCount() & 0xFF;
    pDest[5] = packet.GetDataType();

    // make sure type is valid
    size_t siTypeSize = rovecomm::DataTypeSize(packet.GetDataType());
    if (siTypeSize == -1)
    {
        std::cout << "unknown type!" << std::endl;
        delete[] pDest;
        return {nullptr, 0};
    }
    char* pBegin = &pDest[6];
    char* pEnd   = pBegin + packet.GetDataCount() * siTypeSize;
    // sizeof double, the largest size
    char pSwap[8] = {0};
    for (char* it = pBegin; it < pEnd; it += siTypeSize)
    {
        std::memcpy(pSwap, it, siTypeSize);
        for (int byte = 0; byte < siTypeSize; byte++)
        {
            it[byte] = pSwap[siTypeSize - byte - 1];
        }
    }

    return RoveCommPacketBuffer{std::unique_ptr<char>(pDest), packet.GetSize()};
}

const RoveCommPacket RoveCommPacket::Unpack(const char* source)
{
    if (source.GetSize() < rovecomm::ROVECOMM_PACKET_HEADER_SIZE)
    {
        std::cout << "Could not read packet! Not enough bytes." << std::endl;
        return RoveCommPacket::NONE;
    }
    RoveCommPacketHeader sHeader;
    source.Flip();
    source.Get(sHeader.ucVersionId);
    source.Get(sHeader.usDataId);
    source.Get(sHeader.usDataCount);
    source.Get(sHeader.ucDataType);

    // Convert short values to correct byte order
    sHeader.usDataId    = ntohs(sHeader.usDataId);
    sHeader.usDataCount = ntohs(sHeader.usDataCount);

    // Make sure there is enough data to read
    size_t promisedLength = sHeader.usDataCount * rovecomm::DataTypeSize(sHeader.ucDataType), actualLength = source.GetSize() - rovecomm::ROVECOMM_PACKET_HEADER_SIZE;

    if (actualLength != promisedLength)
    {
        std::cout << "Could not read packet! Promised " << promisedLength << " bytes, but only found " << actualLength << '.' << std::endl;
        return RoveCommPacket::NONE;
    }

    RoveCommByteBuffer buffer(actualLength);

    switch (sHeader.ucDataType)
    {
        case rovecomm::DataTypes::INT8_T:
        case rovecomm::DataTypes::UINT8_T:
        case rovecomm::DataTypes::CHAR:
        {
            int8_t d;
            for (int i = 0; i < sHeader.usDataCount; i++)
            {
                source.Get(d);
                buffer.Put(d);
            }
            break;
        }
        case rovecomm::DataTypes::INT16_T:
        case rovecomm::DataTypes::UINT16_T:
        {
            int16_t d;
            for (int i = 0; i < sHeader.usDataCount; i++)
            {
                source.Get(d);
                buffer.Put(ntohs(d));
            }
            break;
        }
        case rovecomm::DataTypes::INT32_T:
        case rovecomm::DataTypes::UINT32_T:
        {
            int32_t d;
            for (int i = 0; i < sHeader.usDataCount; i++)
            {
                source.Get(d);
                buffer.Put(ntohl(d));
            }
            break;
        }
        case rovecomm::DataTypes::DOUBLE_T:
        {
            // IDK what to do here
            break;
        }
        case rovecomm::DataTypes::FLOAT_T:
        {
            // IDK
            break;
        }
        default:
        {
            std::cout << "Unknown data type!" << std::endl;
        }
    }

    return RoveCommPacket(sHeader, std::unique_ptr<char>(buffer.Release()));
}

std::ostream& operator<<(std::ostream& out, const RoveCommPacket& packet)
{
    out << "----------";
    out << "ID: " << packet.GetDataId() << "\n";
    out << "Count: " << packet.GetDataCount() << "\n";
    out << "Type: " << packet.GetDataType() << "\n";
    out << "Data: [ " << packet.GetDataCount() * rovecomm::DataTypeSize(packet.GetDataType()) << " bytes ]\n";
    out << "----------\n";
    return out;
}
