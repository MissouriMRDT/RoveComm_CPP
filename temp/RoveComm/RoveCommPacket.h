/******************************************************************************
 * @brief RoveComm Packet Definition.
 *
 * @file RoveCommPacket.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-05
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_PACKET_H
#define ROVECOMM_PACKET_H

#define ROVECOMM_ETHERNET_UDP_MAX_SUBSCRIBERS 10
#define ROVECOMM_PACKET_MAX_DATA_COUNT        65535
#define ROVECOMM_PACKET_HEADER_SIZE           6
#define ROVECOMM_VERSION                      3

#include "./RoveCommManifest.h"

#include <cstdint>
#include <cstring>
#include <vector>

class RoveCommPacket
{
    public:
        uint16_t data_id;
        uint16_t data_count;
        manifest::DataTypes data_type;
        std::vector<uint8_t> data;

        // Default Constructor
        RoveCommPacket();

        // Constructor for creating a packet
        RoveCommPacket(uint16_t id, uint16_t count, manifest::DataTypes type, const std::vector<uint8_t>& payload);

        // Display packet information
        void displayPacket() const;

        // Pack data into the packet
        template<typename T>
        static RoveCommPacket pack(const uint16_t data_id, const std::vector<T>& data);

        // Unpack data from the packet
        template<typename T>
        std::vector<T> unpack() const;
};

#endif    // ROVECOMM_PACKET_H
