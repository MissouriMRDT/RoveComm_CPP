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
#include "Consts.h"

RoveCommPacket::RoveCommPacket(int nDataId = 0, char cDataType = 'b', int aData[], std::string szIp = "", int nPort = ROVECOMM_UDP_PORT)
{
    this->m_nDataId   = nDataId;
    this->m_cDataType = cDataType;
    // int data_count  = std::size(data);
    // this->m_aData = aData;
    //  IP should be the full IP address
    //   in case of empty IP default to unknown IP
    if (szIp != "")
    {
        m_stIp = {szIp, nPort};
    }
    else
    {
        m_stIp = {"0.0.0.0", 0};
    }

    return;
    /*
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
}

void RoveCommPacket::SetIp(std::string szIp, int nPort = NULL)
{
    return;
}

void RoveCommPacket::Print()
{
    return;
}
