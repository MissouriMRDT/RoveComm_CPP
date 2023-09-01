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

namespace constants
{
    const int ROVECOMM_ETHERNET_UDP_MAX_SUBSCRIBERS = 10;
    const int ROVECOMM_PACKET_MAX_DATA_COUNT        = 65535;
    const int ROVECOMM_PACKET_HEADER_SIZE           = 6;
    const int ROVECOMM_VERSION                      = 3;

    const unsigned int UDP_PORT                     = 11000;
    const unsigned int TCP_PORT                     = 12000;

}    // namespace constants

#endif    // CONSTS_H
