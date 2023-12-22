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

#include <iostream>

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

size_t RoveCommPacket::size() const
{
    return rovecomm::ROVECOMM_PACKET_HEADER_SIZE + (m_sHeader.unDataCount * rovecomm::DataTypeSize(m_sHeader.unDataType));
}

std::ostream& operator<<(std::ostream& out, const RoveCommPacket& packet)
{
    out << "----------";
    out << "ID: " << packet.GetDataId() << "\n";
    out << "Count: " << packet.GetDataCount() << "\n";
    out << "Type: " << packet.GetDataType() << "\n";
    out << "Data: [ " << packet.GetDataCount() * rovecomm::DataTypeSize(packet.GetDataType()) << " bytes ]\n";
    out << "----------" << '\n';
    return out;
}

/* unused for now
        self.data_id = data_id
        self.data_type = data_type
        self.data_count = len(data)
        self.data = data
        # If the ip is less than 4 chars we know it's the 4th octet, otherwise it's the full IP
        # in case of empty IP default to unknow IP
        if ip_octet_4 != "" and len(ip_octet_4) < 4:
            self.ip_address = ("192.168.1." + ip_octet_4, port)
        elif ip_octet_4 != "" and len(ip_octet_4) >= 4:
            self.ip_address = (ip_octet_4, port)
        else:
            self.ip_address = ("0.0.0.0", 0)
        return
*/
