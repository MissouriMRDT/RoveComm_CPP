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
#include "function"

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
    void setIp(std::string ip, int port=NULL);
    void print();
    struct ip_address{
        string ip;
        int port;
    };
};

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
    RoveComm(int udp_port, int tcp_addr);
    void listen();
    void set_callback(int data_id, std::function& func);
    void clear_callback(int data_id);
    void set_default_callback(std::function& func);
    void clear_default_callback();
    int write(RoveCommPacket& packet, bool reliable=False);
    void close_thread();
}

class RoveCommEthernetUdp
/*
    The UDP implementation for RoveComm. UDP is a fast connectionless transport
    protocol that guarantees no data corruption but does not guarantee delivery,
    and if it delivers does not guarantee it being in the same order it was
    sent.

    Methods:
    --------
        write(packet):
            Transmits a packet to the destination IP and all active subscribers.
        read():
            Unpacks the UDP packet and packs it into a RoveComm Packet for easy
            parsing in other code.
        subscribe(ip_octet):
            Subscribes to UDP packets from the given ip
        close_socket():
            Closes the UDP socket
*/
{
    RoveCommEthernetUdp(int port=ROVECOMM_UDP_PORT);
    int subscribe(std::string sub_to_ip);
    int write(RoveCommPacket& packet);
    RoveCommPacket& read();
    void close_socket();
}

class RoveCommEthernetTcp
/*
    The TCP implementation for RoveComm.

    Methods:
    --------
        write(packet):
            Transmits a packet to the destination IP and all active subscribers.
        read():
            Receives all TCP packets from open sockets and packs data into RoveCommPacket instances
        connect(ip_octet):
            Opens a socket connection to the given address
        close_sockets():
            Closes the server socket and all open sockets
        handle_incoming_connections():
            Accepts socket connection requests
*/
{
    RoveCommEthernetTcp(std::string HOST, int PORT=ROVECOM_TCP_PORT);
    void close_sockets();
    int write(RoveCommPacket& packet);
    int connect(std::string address);
    void handle_incoming_connection();
    RoveCommPacket[]& read();
}

std::map<std::string, std::string> get_manifest(std::string path="");

#endif    // ROVECOMM_H
