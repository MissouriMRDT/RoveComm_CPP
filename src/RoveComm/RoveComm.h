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
#include "string"
#include <map>

// FIXME: These maps should be moved to the constants namespace
std::map<int, char> types_int_to_byte  = {{0, 'b'}, {1, 'B'}, {2, 'h'}, {3, 'H'}, {4, 'l'}, {5, 'L'}, {6, 'f'}, {7, 'd'}, {8, 'c'}};

std::map<char, int> types_byte_to_int  = {{'b', 0}, {'B', 1}, {'h', 2}, {'H', 3}, {'l', 4}, {'L', 5}, {'f', 6}, {'d', 7}, {'c', 8}};

std::map<char, int> types_byte_to_size = {{'b', 1}, {'B', 1}, {'h', 2}, {'H', 2}, {'l', 4}, {'L', 4}, {'f', 4}, {'q', 8}, {'d', 8}, {'c', 1}};

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
        // FIXME: Change parameter variables names to meet our style guide.
        RoveComm(int udp_port, int tcp_addr);

        // FIXME: Change function names and parameter variable names to meet our style guide.
        void listen();
        void set_callback(int data_id, std::string& func);    // Find a way to pass function as argument
        void clear_callback(int data_id);
        void set_default_callback(std::string& func);         // Find a way to pass function as argument
        void clear_default_callback();
        int write(RoveCommPacket& packet, bool reliable = false);
        void close_thread();
}

// FIXME: Change function names and paramerter variable names to meet our style guide.
// std::map<std::string, std::string>
// get_manifest(std::string path = "");

#endif    // ROVECOMM_H
