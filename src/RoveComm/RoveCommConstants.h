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

#ifndef ROVECOMM_CONSTS_H
#define ROVECOMM_CONSTS_H

namespace rovecomm
{
    const int ROVECOMM_ETHERNET_UDP_MAX_SUBSCRIBERS = 10;
    const int ROVECOMM_PACKET_MAX_BYTES             = 65535;    // this should be 65507 bytes because UDP and IPv4 add their own headers
    const int ROVECOMM_PACKET_HEADER_SIZE           = 6;
    const int ROVECOMM_VERSION                      = 3;
    const int ROVECOMM_ETHERNET_TCP_MAX_CONNECTIONS = 5;

    /******************************************************************************
     * @brief RoveComm System Information
     *
     * @author Missouri S&T - Mars Rover Design Team
     * @date 2023-10-21
     ******************************************************************************/
    namespace System
    {
        // pass this value to functions listening for data id's to signal to listen for all data id's
        // this variable is in the rovecomm::System namespace to be with other values like SUBSCRIBE_DATA_ID
        // this variable is not in RoveCommManifest.h because this constant is not present in the manifests
        // of other RoveComm implementations
        const int ANY_DATA_ID = 0;
    }    // namespace System
}    // namespace rovecomm
#endif    // ROVECOMM_CONSTS_H
