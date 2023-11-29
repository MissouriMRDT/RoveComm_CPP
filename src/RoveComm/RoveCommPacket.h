/******************************************************************************
 * @brief RoveCommPacket Implementation for RoveComm C++ Library
 *
 * @file RoveCommPacket.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-10-08
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_PACKET_H
#define ROVECOMM_PACKET_H

#include "RoveCommConstants.h"
#include "RoveCommManifest.h"

#include <ostream>
#include <stdlib.h>
#include <string>

using RoveCommVersionId = uint8_t;
using RoveCommDataId    = uint16_t;
using RoveCommDataCount = uint16_t;
using RoveCommDataType  = rovecomm::DataTypes;

/*
 *  Header format (sits under TCP/UDP header):
 *       0      7 8     15 16    23 24    31 32    39 40    47 48
 *      +--------+--------+--------+--------+--------+--------+-- ...
 *      |version |intended use (id)| # of elements   | type   | the actual data
 *      +--------+--------+--------+--------+--------+--------+-- ...
 *
 */

/******************************************************************************
 * @brief internal typedef to make the RoveCommPacketHeader union work
 *
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-29
 ******************************************************************************/
typedef struct
{
        RoveCommVersionId unVersionId;
        RoveCommDataId unDataId;
        RoveCommDataCount unDataCount;
        RoveCommDataType eDataType;
} RoveCommPacketHeader_t;

/******************************************************************************
 * @brief A convenient tool for reading a RoveCommPacketHeader as a char array
 * Meant to be internal, but you can use it if you want I guess
 *
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-29
 ******************************************************************************/
typedef union
{
        RoveCommPacketHeader_t fields;
        char bytes[sizeof(RoveCommPacketHeader_t)];
} RoveCommPacketHeader;

/******************************************************************************
 * @brief The RoveComm packet is the encapsulation of a message sent across the rover
 *  network that can be parsed by all rover computing systems.
 *
 *  In this implementation, packets are immutable state.
 *
 ******************************************************************************/
class RoveCommPacket
{
    public:
        RoveCommPacket();
        RoveCommPacket(unsigned int unDataId, rovecomm::DataTypes eDataType, void* aData);

        inline RoveCommVersionId getVersionId() const { return m_uHeader.fields.unVersionId; }

        inline RoveCommDataId getDataId() const { return m_uHeader.fields.unDataId; }

        inline RoveCommDataCount getDataCount() const { return m_uHeader.fields.unDataCount; }

        inline rovecomm::DataTypes getDataType() const { return m_uHeader.fields.eDataType; }

        inline void* getData() const { return m_aData; }

    private:
        RoveCommPacketHeader m_uHeader;
        void* m_aData;    // consider making this an std::array<int> for ease of use
};

// print with std::cout
inline std::ostream& operator<<(std::ostream& out, const RoveCommPacket& packet);

#endif    // ROVECOMM_PACKET_H
