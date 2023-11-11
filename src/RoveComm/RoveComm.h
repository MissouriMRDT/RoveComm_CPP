/******************************************************************************
 * @brief Main Header for RoveComm C++
 *
 * @file RoveComm.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-07-29
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_H
#define ROVECOMM_H

#include "Consts.h"
#include "RoveCommEthernetTcp.h"
#include "RoveCommEthernetUdp.h"
#include "RoveCommPacket.h"

#include <functional>
#include <map>
#include <string>

class RoveComm
/*
    Creates a separate thread to read all RoveComm connections

    Methods:
    --------
        write(packet, reliable):
            Writes the given packet to its destination address
        set_callback(data_id, func):
            Sets the callback function for any incoming packets with the given data id
        close_thread():
            Shuts down the listener thread
*/
{
    public:
        std::map<int, std::function<void(RoveCommPacket*)>*> Callbacks;
        std::function<void(RoveCommPacket*)>* DefaultCallback;
        RoveCommEthernetUdp UdpNode;
        RoveCommEthernetTcp TcpNode;

        RoveComm(int nUdpPort, ip_address stTcpAddr);

        void Listen();
        void SetCallback(int nDataId, std::function<void(RoveCommPacket*)>* Func);
        void ClearCallback(int nDataId);
        void SetDefaultCallback(std::function<void(RoveCommPacket*)>* Func);
        void ClearDefaultCallback();
        int Write(RoveCommPacket& Packet, bool bReliable = false);
        void CloseThread();
};

#endif    // ROVECOMM_H
