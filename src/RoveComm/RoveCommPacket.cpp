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

// RoveCommPacket::RoveCommPacket(rovecomm::ManifestEntry sEntry, void* pData): RoveCommPacket({rovecomm::ROVECOMM_VERSION, sEntry

void RoveCommPacket::WriteHeader(char* pDest, const RoveCommPacketHeader& header)
{
    pDest[0] = header.ucVersionId;
    pDest[1] = header.usDataId >> 8;    // convert to network order
    pDest[2] = header.usDataId & 0xFF;
    pDest[3] = header.usDataCount >> 8;
    pDest[4] = header.usDataCount & 0xFF;
    pDest[5] = header.ucDataType;
    if (header.ucVersionId != rovecomm::ROVECOMM_VERSION)
    {
        throw std::runtime_error("Tried to write header of unknown version.");
    }
    if (rovecomm::DataTypeSize(header.ucDataType) == 0)
    {
        throw std::runtime_error("Tried to write header of unknown type.");
    }
}

void RoveCommPacket::ReadHeader(RoveCommPacketHeader& dest, const char* pSource)
{
    dest.ucVersionId = pSource[0];
    dest.usDataId    = pSource[1] << 8 | pSource[2];
    dest.usDataCount = pSource[3] << 8 | pSource[4];
    dest.ucDataType  = pSource[5];

    if (dest.ucVersionId != rovecomm::ROVECOMM_VERSION)
    {
        throw std::runtime_error("Tried to read header of unknown version.");
    }
    if (rovecomm::DataTypeSize(dest.ucDataType) == 0)
    {
        throw std::runtime_error("Tried to read header of known version.");
    }
}

void RoveCommPacket::WriteData(char* pDest, const RoveCommPacket& packet)
{
    // make sure version is valid
    if (packet.m_sHeader.ucVersionId != rovecomm::ROVECOMM_VERSION)
    {
        throw std::runtime_error("Tried to serialize packet of unknown version.");
    }
    // make sure type is valid
    size_t siTypeSize = rovecomm::DataTypeSize(packet.m_sHeader.ucDataType);
    if (siTypeSize == 0)
    {
        throw std::runtime_error("Tried to serialize packet of unknown type.");
    }

    char* pData = packet.m_pData.get();
    for (int el = 0; el < packet.m_sHeader.usDataCount * siTypeSize; el++)
    {
        for (int byte = 0; byte < siTypeSize; byte++)
        {
            pDest[el * siTypeSize + byte] = pData[el * siTypeSize + siTypeSize - byte - 1];
        }
    }
}

void RoveCommPacket::ReadData(char* pDest, const char* pSource, const RoveCommPacketHeader& header)
{
    if (header.ucVersionId != rovecomm::ROVECOMM_VERSION)
    {
        throw std::runtime_error("Tried to deserialize packet of unknown version.");
    }
    size_t siTypeSize = rovecomm::DataTypeSize(header.ucDataType);
    if (siTypeSize == 0)
    {
        throw std::runtime_error("Tried to deserialize packet of unknown type.");
    }
    for (int el = 0; el < header.usDataCount * siTypeSize; el++)
    {
        for (int byte = 0; byte < siTypeSize; byte++)
        {
            pDest[el * siTypeSize + byte] = pSource[el * siTypeSize + siTypeSize - byte - 1];
        }
    }
}

RoveCommPacketBuffer RoveCommPacket::Pack(const RoveCommPacket& packet)
{
    size_t siSize = packet.GetSize();
    char* pData   = new char[siSize];
    try
    {
        RoveCommPacket::WriteHeader(pData, packet.m_sHeader);
        RoveCommPacket::WriteData(pData, packet);
    }
    catch (std::runtime_error& eErr)
    {
        delete[] pData;
        throw eErr;
    }
    return {std::unique_ptr<char>(pData), siSize};
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
