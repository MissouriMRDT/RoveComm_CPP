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

std::ostream& operator<<(std::ostream& out, const RoveCommPacket& packet)
{
    out << "----------";
    out << "ID: " << packet.getDataId() << "\n";
    out << "Count: " << packet.getDataCount() << "\n";
    out << "Type: " << packet.getDataType() << "\n";
    std::cout << "Data: [";
    // for (int i=0, i<)
    out << "]\n";
    out << "----------" << std::endl;
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
