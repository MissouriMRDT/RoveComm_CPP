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
#include "string"

struct ip_address
{
        std::string ip;
        int port;
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
        int m_aData[];

        RoveCommPacket(int nDataId = 0, char cDataType = 'b', int aData[], std::string szIp = "", int nPort = ROVECOMM_UDP_PORT);

        void SetIp(std::string szIp, int nPort = NULL);
        void Print();

        // FIXME: This struct shouldn't be a part of the class and should exist at the global scope.
        //        Also, make sure to change variable names to meet our style guide. Note that you
        //        don't need the `m_` for structs.
};

#endif    // ROVECOMM_PACKET_H
