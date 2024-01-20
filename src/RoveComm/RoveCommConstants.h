/******************************************************************************
 * @brief Defines Global Constants within RoveComm C++ that aren't already defined
 * in RoveCommManifest.h
 *
 * @file Consts.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-07-29
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef CONSTS_H
#define CONSTS_H

namespace rovecomm
{
    const int ROVECOMM_ETHERNET_UDP_MAX_SUBSCRIBERS = 10;
    const int ROVECOMM_PACKET_MAX_DATA_COUNT        = 65535;    // this should be a smaller number
    const int ROVECOMM_PACKET_HEADER_SIZE           = 6;
    const int ROVECOMM_VERSION                      = 3;
    const int ROVECOMM_ETHERNET_TCP_MAX_CONNECTIONS = 5;

    namespace System
    {
        // pass to RoveCommServer::Fetch() signifying any data id. This is not a valid data id.
        const int ANY = 0;
    }    // namespace System
}    // namespace rovecomm
#endif    // CONSTS_H
