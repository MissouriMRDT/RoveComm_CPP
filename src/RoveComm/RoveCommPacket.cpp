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
#include <iostream>

RoveCommPacket::RoveCommPacket(int nDataId, char cDataType, std::string szIp, int nPort, int* aData)
{
    this->m_nDataId   = nDataId;
    this->m_cDataType = cDataType;
    //int data_count  = std::size(data);
    aData = this->m_aData;
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

void RoveCommPacket::SetIp(std::string szIp, int nPort /*= 11000*/)
{
    
    if(nPort == 11000)
    {
        nPort = m_stIp.nPort;
    }
    m_stIp = {szIp, nPort};
    return;
}

void RoveCommPacket::Print()
{
    std::cout << "----------" << std::endl;
    std::cout << "ID: " << m_nDataId << std::endl;
    std::cout << "Type: " << m_cDataType << std::endl;
    std::cout << "IP: (" << m_stIp.stIp.sa_data << ", " << m_stIp.nPort << ")" << std::endl;
    std::cout << "Data: [";
    for(int i=0; i < m_nDataCount; i++)
    {
        
        std::cout << m_aData[i];
        if(i != m_nDataCount - 1)
        {
            std::cout << ", ";
        }
        
    }

    std::cout << "]" << std::endl;
    std::cout << "----------" << std::endl;
    return;
}
