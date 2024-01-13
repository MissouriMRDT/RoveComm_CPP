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
#include "RoveCommByteBuffer.hpp"
#include "RoveCommConstants.h"
#include "RoveCommHelpers.h"

#include <iostream>
#include <memory>
#include <netinet/in.h>

size_t rovecomm::DataTypeSize(RoveCommDataType unType)
{
    switch (unType)
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
            std::cout << "Unknown data type!" << std::endl;
            return -1;
        }
    }
}

RoveCommByteBuffer RoveCommPacket::Pack(const RoveCommPacket& packet)
{
    // Write header first
    RoveCommByteBuffer dest(packet.GetSize());
    dest.Put(packet.GetVersionId());
    dest.Put(htons(packet.GetDataId()));    // convert to network order
    dest.Put(htons(packet.GetDataCount()));
    dest.Put(packet.GetDataType());

    switch (packet.GetDataType())
    {
        case rovecomm::DataTypes::INT8_T:
        case rovecomm::DataTypes::UINT8_T:
        case rovecomm::DataTypes::CHAR:
        {
            int8_t d;
            for (int i = 0; i < packet.GetDataCount(); i++)
            {
                packet.Get(d, i);
                dest.Put(d);
            }
            break;
        }
        case rovecomm::DataTypes::INT16_T:
        case rovecomm::DataTypes::UINT16_T:
        {
            int16_t d;
            for (int i = 0; i < packet.GetDataCount(); i++)
            {
                packet.Get(d, i);
                dest.Put(htons(d));
            }
            break;
        }
        case rovecomm::DataTypes::INT32_T:
        case rovecomm::DataTypes::UINT32_T:
        {
            int32_t d;
            for (int i = 0; i < packet.GetDataCount(); i++)
            {
                packet.Get(d, i);
                dest.Put(htonl(d));
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
    return dest;
}

const RoveCommPacket RoveCommPacket::Unpack(RoveCommByteBuffer& source)
{
    if (source.GetSize() < rovecomm::ROVECOMM_PACKET_HEADER_SIZE)
    {
        std::cout << "Could not read packet! Not enough bytes." << std::endl;
        return RoveCommPacket::NONE;
    }
    RoveCommPacketHeader sHeader;
    source.Flip();
    source.Get(sHeader.unVersionId);
    source.Get(sHeader.unDataId);
    source.Get(sHeader.unDataCount);
    source.Get(sHeader.unDataType);

    // Convert short values to correct byte order
    sHeader.unDataId    = ntohs(sHeader.unDataId);
    sHeader.unDataCount = ntohs(sHeader.unDataCount);

    // Make sure there is enough data to read
    size_t promisedLength = sHeader.unDataCount * rovecomm::DataTypeSize(sHeader.unDataType), actualLength = source.GetSize() - rovecomm::ROVECOMM_PACKET_HEADER_SIZE;

    if (actualLength != promisedLength)
    {
        std::cout << "Could not read packet! Promised " << promisedLength << " bytes, but only found " << actualLength << '.' << std::endl;
        return RoveCommPacket::NONE;
    }

    RoveCommByteBuffer buffer(actualLength);

    switch (sHeader.unDataType)
    {
        case rovecomm::DataTypes::INT8_T:
        case rovecomm::DataTypes::UINT8_T:
        case rovecomm::DataTypes::CHAR:
        {
            int8_t d;
            for (int i = 0; i < sHeader.unDataCount; i++)
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
            for (int i = 0; i < sHeader.unDataCount; i++)
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
            for (int i = 0; i < sHeader.unDataCount; i++)
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
