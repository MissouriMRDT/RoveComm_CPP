/******************************************************************************
 * @brief RoveCommPacket Implementation for RoveComm C++ Library
 *
 * @file RoveCommPacket.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-10-08
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_PACKET_H
#define ROVECOMM_PACKET_H

#include "Consts.h"

#include <string>

struct ip_address
{
        sockaddr stIp;
        int nPort;
};

class RoveCommPacket
/*
    The RoveComm packet is the encapsulation of a message sent across the rover
    network that can be parsed by all rover computing systems.

    A RoveComm Packet contains:
        - A data id
        - A data type
        - The number of data entries (data count)
        - The data itself

    The autonomy implementation also includes the remote ip of the sender.

    Methods:
    --------
        SetIp(ip, port):
            Sets packet's IP to address parameter
        print():
            Prints the packet'c contents
*/
{
    public:
        int m_nDataId;
        char m_cDataType;
        int m_nDataCount;
        // FIXME: Find the actual size of the data array
        static const int m_nMaxData = 9;
        int m_aData[m_nMaxData];
        struct ip_address m_stIp;

        RoveCommPacket(int nDataId = 0, char cDataType = 'b', std::string szIp = "", int nPort = ROVECOMM_UDP_PORT, int* aData = nullptr);

        void SetIp(std::string szIp, int nPort = 11000);
        void Print();
};

#endif    // ROVECOMM_PACKET_H
