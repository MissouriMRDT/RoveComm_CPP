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
#define ROVECOMM_TCP_PORT                     12000
#define ROVECOMM_VERSION                      3
#define ROVECOMM_HEADER_FORMAT                ">BHHB"

#define ROVECOMM_PING_REQUEST                 1
#define ROVECOMM_PING_REPLY                   2
#define ROVECOMM_SUBSCRIBE_REQUEST            3
#define ROVECOMM_UNSUBSCRIBE_REQUEST          4
#define ROVECOMM_INCOMPATIBLE_VERSION         5

#endif    // CONSTS_H
