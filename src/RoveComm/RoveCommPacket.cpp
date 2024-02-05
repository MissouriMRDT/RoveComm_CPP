/******************************************************************************
 * @brief
 *
 * @file RoveCommPacket.cpp
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-05
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#include "RoveCommPacket.h"
#include <iostream>

// Default Constructor
RoveCommPacket::RoveCommPacket()
{
    data_id    = 0;
    data_count = 0;
    data_type  = manifest::DataTypes::UINT8_T;
}

// Constructor for creating a packet
RoveCommPacket::RoveCommPacket(uint16_t id, uint16_t count, manifest::DataTypes type, const std::vector<uint8_t>& payload) :
    data_id(id), data_count(count), data_type(type), data(payload)
{}

// Display packet information
void RoveCommPacket::displayPacket() const
{
    std::cout << "Data ID: " << data_id << std::endl;
    std::cout << "Data Count: " << data_count << std::endl;
    std::cout << "Data Type: " << static_cast<uint8_t>(data_type) << std::endl;
    std::cout << "Data Content: ";
    for (const auto& byte : data)
    {
        std::cout << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;
}

// Pack data into the packet
template<typename T>
RoveCommPacket RoveCommPacket::pack(const uint16_t data_id, const std::vector<T>& data)
{
    // Calculate the size of the payload
    size_t payloadSize = data.size() * sizeof(T);

    // Create a packet with the correct size
    RoveCommPacket packet(data_id, static_cast<uint16_t>(data.size()), manifest::DataTypes::UINT8_T, std::vector<uint8_t>(payloadSize));

    // Copy the data into the packet's payload
    std::memcpy(packet.data.data(), data.data(), payloadSize);

    return packet;
}

// Unpack data from the packet
template<typename T>
std::vector<T> RoveCommPacket::unpack() const
{
    // Calculate the size of the payload
    size_t payloadSize = data.size();

    // Calculate the number of elements in the payload
    size_t numElements = payloadSize / sizeof(T);

    // Create a vector to hold the unpacked data
    std::vector<T> unpackedData(numElements);

    // Copy the data from the packet's payload
    std::memcpy(unpackedData.data(), data.data(), payloadSize);

    return unpackedData;
}

// Explicit instantiation of the template for supported types
template RoveCommPacket RoveCommPacket::pack(const uint16_t data_id, const std::vector<uint8_t>& data);
template RoveCommPacket RoveCommPacket::pack(const uint16_t data_id, const std::vector<uint16_t>& data);
template RoveCommPacket RoveCommPacket::pack(const uint16_t data_id, const std::vector<uint32_t>& data);
template RoveCommPacket RoveCommPacket::pack(const uint16_t data_id, const std::vector<float>& data);
template RoveCommPacket RoveCommPacket::pack(const uint16_t data_id, const std::vector<double>& data);
template RoveCommPacket RoveCommPacket::pack(const uint16_t data_id, const std::vector<char>& data);

// Explicit instantiation of the template for supported types
template std::vector<uint8_t> RoveCommPacket::unpack<uint8_t>() const;
template std::vector<uint16_t> RoveCommPacket::unpack<uint16_t>() const;
template std::vector<uint32_t> RoveCommPacket::unpack<uint32_t>() const;
template std::vector<float> RoveCommPacket::unpack<float>() const;
template std::vector<double> RoveCommPacket::unpack<double>() const;
template std::vector<char> RoveCommPacket::unpack<char>() const;
