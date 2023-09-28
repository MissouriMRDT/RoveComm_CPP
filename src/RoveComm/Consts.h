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

#define ROVECOMM_ETHERNET_UDP_MAX_SUBSCRIBERS 10
#define ROVECOMM_PACKET_MAX_DATA_COUNT        65535
#define ROVECOMM_PACKET_HEADER_SIZE           6
#define ROVECOMM_VERSION                      3

#define ROVECOMM_UDP_PORT                     11000
/*
const int ROVECOMM_UDP_PORT = 11000;
const int ROVECOMM_TCP_PORT = 12000;
const int ROVECOMM_VERSION = 3;
const std::string ROVECOMM_HEADER_FORMAT = ">BHHB";

const int ROVECOMM_PING_REQUEST = 1;
const int ROVECOMM_PING_REPLY = 2;
const int ROVECOMM_SUBSCRIBE_REQUEST = 3;
const int ROVECOMM_UNSUBSCRIBE_REQUEST = 4;
const int ROVECOMM_INCOMPATIBLE_VERSION = 5;
*/
#endif    // CONSTS_H
