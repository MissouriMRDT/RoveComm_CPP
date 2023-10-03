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

#include "RoveComm.hpp"
#include "Consts.h"
#include <map>
#include "string"

std::map<int, char>types_int_to_byte = {
    {0, 'b'},
    {1, 'B'},
    {2, 'h'},
    {3, 'H'},
    {4, 'l'},
    {5, 'L'},
    {6, 'f'},
    {7, 'd'},
    {8, 'c'}
}; 

std::map<char, int>types_byte_to_int = {
    {'b', 0},
    {'B', 1},
    {'h', 2},
    {'H', 3},
    {'l', 4},
    {'L', 5},
    {'f', 6},
    {'d', 7},
    {'c', 8}
}; 

std::map<char, int>types_byte_to_size = {
    {'b', 1},
    {'B', 1},
    {'h', 2},
    {'H', 2},
    {'l', 4},
    {'L', 4},
    {'f', 4},
    {'q', 8},
    {'d', 8},
    {'c', 1}
};

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
    int data_id;
    char data_type;
    int data_count;
    int data[];

	RoveCommPacket(int data_id=0, char data_type='b', int data[], std::string ip="", int port=ROVECOMM_UDP_PORT);

};

std::map<std::string, std::string> get_manifest(std::string path="");

#endif    // ROVECOMM_H
