/******************************************************************************
 * @brief Defines Global Constants within RoveComm C++
 *
 * @file Consts.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-07-29
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef CONSTS_H
#define CONSTS_H

#include <map>
#include <string>

const int ROVECOMM_ETHERNET_UDP_MAX_SUBSCRIBERS = 10;
const int ROVECOMM_PACKET_MAX_DATA_COUNT        = 65535;
const int ROVECOMM_PACKET_HEADER_SIZE           = 6;
const int ROVECOMM_VERSION                      = 3;

const int ROVECOMM_UDP_PORT                     = 11000;
const int ROVECOMM_TCP_PORT                     = 12000;
const std::string ROVECOMM_HEADER_FORMAT        = ">BHHB";

const int ROVECOMM_PING_REQUEST                 = 1;
const int ROVECOMM_PING_REPLY                   = 2;
const int ROVECOMM_SUBSCRIBE_REQUEST            = 3;
const int ROVECOMM_UNSUBSCRIBE_REQUEST          = 4;
const int ROVECOMM_INCOMPATIBLE_VERSION         = 5;

std::map<int, char> TYPES_INT_TO_BYTE           = {{0, 'b'}, {1, 'B'}, {2, 'h'}, {3, 'H'}, {4, 'l'}, {5, 'L'}, {6, 'f'}, {7, 'd'}, {8, 'c'}};

std::map<char, int> TYPES_BYTE_TO_INT           = {{'b', 0}, {'B', 1}, {'h', 2}, {'H', 3}, {'l', 4}, {'L', 5}, {'f', 6}, {'d', 7}, {'c', 8}};

std::map<char, int> TYPES_BYTE_TO_SIZE          = {{'b', 1}, {'B', 1}, {'h', 2}, {'H', 2}, {'l', 4}, {'L', 4}, {'f', 4}, {'q', 8}, {'d', 8}, {'c', 1}};

#endif    // CONSTS_H
