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
 * @brief The RoveComm packet is an immutable encapsulation of a message sent across the rover
 *  network that can be parsed by all rover computing systems.
 *
 *  In this implementation, packets are immutable state.
 *
 ******************************************************************************/
class RoveCommPacket
{
    public:
        RoveCommPacket() : RoveCommPacket(rovecomm::System::NO_DATA_DATA_ID, 0, RoveCommDataType::INT8_T, nullptr){};
        RoveCommPacket(RoveCommDataId unDataId, RoveCommDataCount unDataCount, RoveCommDataType eDataType, const char* aData) :
            m_uHeader({RoveCommPacketHeader_t{rovecomm::ROVECOMM_VERSION, unDataId, unDataCount, eDataType}}), m_aData(aData){};

        // TODO: copy constructors and move semantics
        //  RoveCommPacket(const RoveCommPacket& other);
        //  RoveCommPacket operator=(const RoveCommPacket&& other);

        inline RoveCommVersionId GetVersionId() const { return m_uHeader.fields.unVersionId; }

        inline RoveCommDataId GetDataId() const { return m_uHeader.fields.unDataId; }

        inline RoveCommDataCount GetDataCount() const { return m_uHeader.fields.unDataCount; }

        inline RoveCommDataType GetDataType() const { return m_uHeader.fields.eDataType; }

        inline const char* GetData() const { return m_aData; }

        // An empty packet you can compare to I guess
        static const RoveCommPacket NONE;

    private:
        const RoveCommPacketHeader m_uHeader;
        const char* m_aData;
        // const char m_aData[rovecomm::ROVECOMM_PACKET_MAX_DATA_COUNT];    // consider making this an std::array<int> for ease of use
};

// print with std::cout
inline std::ostream& operator<<(std::ostream& out, const RoveCommPacket& packet);

#endif    // ROVECOMM_PACKET_H
