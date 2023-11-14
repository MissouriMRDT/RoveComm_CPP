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

        inline RoveCommVersionId getVersionId() const { return m_unVersionId; }

        inline RoveCommDataId getDataId() const { return m_unDataId; }

        inline RoveCommDataCount getDataCount() const { return m_unDataCount; }

        inline rovecomm::DataTypes getDataType() const { return m_eDataType; }

        inline void* getData() const { return m_aData; }

    private:
        RoveCommVersionId m_unVersionId;
        RoveCommDataId m_unDataId;
        RoveCommDataCount m_unDataCount;
        RoveCommDataType m_eDataType;
        void* m_aData;    // consider making this an std::array<int> for ease of use
};

// print with std::cout
inline std::ostream& operator<<(std::ostream& out, const RoveCommPacket& packet);

#endif    // ROVECOMM_PACKET_H
