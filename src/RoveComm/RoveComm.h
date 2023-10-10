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
#include "RoveComm.cpp"
#include "RoveCommPacket.h"

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
        RoveComm(int nUdpPort, int nTcpAddr);

        void Listen();
        void SetCallback(int nDataId, std::string& Func);    // Find a way to pass function as argument
        void ClearCallback(int nDataId);
        void SetDefaultCallback(std::string& Func);          // Find a way to pass function as argument
        void ClearDefaultCallback();
        int Write(RoveCommPacket& Packet, bool bReliable = false);
        void CloseThread();
}

// FIXME: Change function names and paramerter variable names to meet our style guide.
// std::map<std::string, std::string>
// get_manifest(std::string path = "");

#endif    // ROVECOMM_H
