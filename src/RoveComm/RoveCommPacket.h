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

#include "RoveCommByteBuffer.hpp"
#include "RoveCommConstants.h"
#include "RoveCommManifest.h"

#include <ostream>
#include <stdlib.h>
#include <string>

using RoveCommVersionId = uint8_t;
using RoveCommDataId    = uint16_t;
using RoveCommDataCount = uint16_t;
using RoveCommDataType  = uint8_t;

namespace rovecomm
{
    size_t DataTypeSize(RoveCommDataType unDataType);
}

/*
 *  Header format (sits under TCP/UDP header):
 *       0      7 8     15 16    23 24    31 32    39 40    47 48
 *      +--------+--------+--------+--------+--------+--------+-- ...
 *      |version |intended use (id)| # of elements   | type   | the actual data
 *      +--------+--------+--------+--------+--------+--------+-- ...
 *
 */

/******************************************************************************
 * @brief Encapsulates header data for a RoveCommPacket. This might be removed later.
 *
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-29
 ******************************************************************************/
struct RoveCommPacketHeader
{
        RoveCommVersionId unVersionId;
        RoveCommDataId unDataId;
        RoveCommDataCount unDataCount;
        RoveCommDataType unDataType;
};

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
        RoveCommPacket() : RoveCommPacket(rovecomm::System::NO_DATA_DATA_ID, 0, rovecomm::DataTypes::INT8_T, nullptr){};
        RoveCommPacket(RoveCommDataId unDataId, RoveCommDataCount unDataCount, RoveCommDataType unDataType, std::unique_ptr<char>&& pData) :
            RoveCommPacket({rovecomm::ROVECOMM_VERSION, unDataId, unDataCount, unDataType}, std::move(pData)){};

        RoveCommPacket(RoveCommPacketHeader sHeader, std::unique_ptr<char>&& pData) :
            m_sHeader(sHeader), m_pData(std::move(pData)), m_dataSize(m_sHeader.unDataCount * rovecomm::DataTypeSize(m_sHeader.unDataType))
        {}

        inline RoveCommVersionId GetVersionId() const { return m_sHeader.unVersionId; }

        inline RoveCommDataId GetDataId() const { return m_sHeader.unDataId; }

        inline RoveCommDataCount GetDataCount() const { return m_sHeader.unDataCount; }

        inline RoveCommDataType GetDataType() const { return m_sHeader.unDataType; }

        // Templates have to be defined in the header or else the linker sh*ts itself

        /******************************************************************************
         * @brief Extract an element from the packet's internal array.
         *
         * @tparam T Type of value to extract (filled in automatically)
         * @param dest Reference in which to store the result.
         * @param index Index of the desired element (scaled to T, not in bytes!)
         * @return size_t - sizeof(T) if successful, 0 if out of range.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-12-23
         ******************************************************************************/
        template<typename T>
        size_t Get(T& dest, int index) const
        {
            if (sizeof(T) * index > m_dataSize)
                return 0;
            dest = *((T*) m_pData) + index;
            return sizeof(T);
        }

        /******************************************************************************
         * @brief Bulk extract elements from the packet's internal array.
         *
         * @tparam T Type of value to extract (filled in automatically)
         * @param pDest Pointer to destination array
         * @param start Index of first element (scaled to T, not in bytes!)
         * @param count How many elements to extract (T, not bytes)
         * @return size_t - How many bytes were successfully read, 0 if out of range.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-12-23
         ******************************************************************************/
        template<typename T>
        size_t Get(T* pDest, int start, int count) const
        {
            size_t bytesToRead = std::min(start + count, m_dataSize / sizeof(T)) * sizeof(T);
            std::memcpy(pDest, &m_pData[start * sizeof(T)], bytesToRead);
        }

        /******************************************************************************
         * @brief Access the internal raw pointer. The pointer will be deleted if the packet goes out of scope!
         * If you just need the data, use RoveCommPacket::Get() instead.
         *
         * @return const char* - The internal raw pointer.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-12-23
         ******************************************************************************/
        const char* GetRawData() const { return m_pData.get(); }

        // inline const std::unique_ptr<const char> GetData() const { return m_pData; }

        /******************************************************************************
         * @brief Get the size of the packet's internal array.
         *
         * @return size_t - Size of the array in bytes.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-12-23
         ******************************************************************************/
        inline size_t GetDataSize() const { return m_dataSize; }

        /******************************************************************************
         * @brief Get the size of the packet (including the header)
         *
         * @return size_t - The size of the packet in bytes.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-12-23
         ******************************************************************************/
        size_t GetSize() const { return rovecomm::ROVECOMM_PACKET_HEADER_SIZE + m_dataSize; }

        /******************************************************************************
         * @brief Pack data into a char array.
         * This is meant to be an internal method.
         *
         * @param packet The packet to pack.
         * @param dest Buffer to write to.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-12-23
         ******************************************************************************/
        static RoveCommByteBuffer Pack(const RoveCommPacket& packet);
        /******************************************************************************
         * @brief Unpack data from a raw char array (in netowrk order) to construct a new RoveCommPacket.
         * This is meant to be an internal method.
         *
         * @param source Buffer to read (in network order)
         * @return RoveCommPacket& The new RoveCommPacket. Returns RoveCommPacket::NONE if there was an error.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-12-22
         ******************************************************************************/
        static const RoveCommPacket Unpack(RoveCommByteBuffer& source);

        // An empty packet you can compare to I guess
        static const RoveCommPacket NONE;

    private:
        const RoveCommPacketHeader m_sHeader;
        const std::unique_ptr<char> m_pData;    // Shared pointer so that packets are copyable
        const size_t m_dataSize;
};

// print with std::cout
inline std::ostream& operator<<(std::ostream& out, const RoveCommPacket& packet);

#endif    // ROVECOMM_PACKET_H
