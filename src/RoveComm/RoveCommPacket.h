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

#include <cstring>
#include <memory>
#include <ostream>
#include <stdarg.h>
#include <stdlib.h>
#include <string>

// using RoveCommVersionId = uint8_t;
// using RoveCommDataId    = uint16_t;
// using RoveCommDataCount = uint16_t;
// using RoveCommDataType  = uint8_t;

namespace rovecomm
{
    /******************************************************************************
     * @brief Get the size of a RoveCommDataType.
     *
     * @param ucDataType - Value to convert
     * @return size_t - Size of the data type. Returns 0 if no matching data type was found.
     *
     * @author OcelotEmpire (hobbz.pi@gmail.com)
     * @date 2024-01-15
     ******************************************************************************/
    size_t DataTypeSize(uint8_t ucDataType);
}    // namespace rovecomm

/*
 *  Header format (sits under TCP/UDP header):
 *       0      7 8     15 16    23 24    31 32    39 40    47 48
 *      +--------+--------+--------+--------+--------+--------+-- ...
 *      |version |intended use (id)| # of elements   | type   | the actual data
 *      +--------+--------+--------+--------+--------+--------+-- ...
 *
 *  Because of memory alignment, RoveCommHeader will look something like:
 *  |aabbbb--|ccccaa--|
 *  with a total size of 8 bytes
 *  so make sure to use rovecomm::ROVECOMM_PACKET_HEADER_SIZE and not
 *  sizeof(RoveCommPacketHeader) when writing data
 */

/******************************************************************************
 * @brief Encapsulates header data for a RoveCommPacket.
 *
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-29
 ******************************************************************************/
struct RoveCommPacketHeader
{
        uint8_t ucVersionId;
        uint16_t usDataId;
        uint16_t usDataCount;
        uint8_t ucDataType;
};

/******************************************************************************
 * @brief Literally only exists to give a unique_ptr a length.
 *
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2024-01-14
 ******************************************************************************/
struct RoveCommPacketBuffer
{
        std::unique_ptr<char> pData;
        size_t siLength;
};

/******************************************************************************
 * @brief The RoveComm packet is an immutable encapsulation of a message sent across the rover
 *  network that can be parsed by all rover computing systems.
 *  These can be copied freely, as the data underneath is backed by an std::shared_pointer<char>.
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-29
 ******************************************************************************/
class RoveCommPacket
{
    public:
        RoveCommPacket() : RoveCommPacket(rovecomm::System::NO_DATA_DATA_ID, 0, rovecomm::DataTypes::INT8_T, nullptr){};

        RoveCommPacket(uint16_t usDataId, uint16_t usDataCount, uint8_t ucDataType, std::unique_ptr<char>&& pData) :
            RoveCommPacket({rovecomm::ROVECOMM_VERSION, usDataId, usDataCount, ucDataType}, std::move(pData)){};

        RoveCommPacket(RoveCommPacketHeader sHeader, std::unique_ptr<char>&& pData) : m_sHeader(sHeader), m_pData(std::move(pData)){};

        // convenience constructors:

        RoveCommPacket(uint16_t usDataId) : RoveCommPacket(usDataId, 0, rovecomm::DataTypes::UINT8_T, std::unique_ptr<char>{}){};

        // example usage: RoveComm.SendTo(address, RoveCommPacket{rovecomm::AUTONOMY::REACHEDMARKER, 1})
        // template<typename T>
        // RoveCommPacket(rovecomm::ManifestEntry sEntry, T data...) :
        //     RoveCommPacket(sEntry.DATA_ID, sEntry.DATA_COUNT, sEntry.DATA_TYPE, std::make_unique<char>(sEntry.DATA_COUNT * rovecomm::DataTypeSize(sEntry.DATA_TYPE)))
        // {
        //     va_list args;
        //     va_start(args, data);
        //     char* pRaw = m_pData.get();
        //     for (int i = 0; i<)
        // };

        inline uint8_t GetVersionId() const { return m_sHeader.ucVersionId; }

        inline uint16_t GetDataId() const { return m_sHeader.usDataId; }

        inline uint16_t GetDataCount() const { return m_sHeader.usDataCount; }

        inline uint8_t GetDataType() const { return m_sHeader.ucDataType; }

        /******************************************************************************
         * @brief Get the size of the packet's data array (not including the header)
         *
         * @return size_t - The size of the packet in bytes
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-20
         ******************************************************************************/
        size_t CalcDataSize() const { return m_sHeader.usDataCount * rovecomm::DataTypeSize(m_sHeader.ucDataType); }

        /******************************************************************************
         * @brief Get the size of the packet (including the header)
         *
         * @return size_t - The size of the packet in bytes.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-12-23
         ******************************************************************************/
        size_t CalcSize() const { return rovecomm::ROVECOMM_PACKET_HEADER_SIZE + CalcDataSize(); }

        // An empty packet you can compare to I guess
        static const RoveCommPacket NONE;

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
            if (sizeof(T) * index >= CalcDataSize())
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
        size_t GetBulk(T* pDest, size_t start, size_t count) const
        {
            size_t bytesToRead = std::min(start + count, CalcDataSize() / sizeof(T)) * sizeof(T);
            std::memcpy(pDest, m_pData.get() + start * sizeof(T), bytesToRead);
            return bytesToRead;
        }

        /******************************************************************************
         * @brief Access the internal raw pointer. The pointer will be deleted with the packet!
         * If you just need the data, use RoveCommPacket::Get() instead.
         *
         * @return const char* - The internal raw pointer.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-12-23
         ******************************************************************************/
        const char* GetRawData() const { return m_pData.get(); }

        /******************************************************************************
         * @brief utility method for checking if a packet's header matches a rovecomm::ManifestEntry.
         * Example: if (packet.MatchesManifestEntry(rovecomm::AUTONOMY::REACHEDMARKER)) { ... }
         *
         * @param entry - the rovecomm::ManifestEntry to compare to
         * @return bool - whether the packet is safe to read in the format described by entry
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-02-05
         ******************************************************************************/
        bool MatchesManifestEntry(const rovecomm::ManifestEntry& entry)
        {
            return m_sHeader.usDataId == entry.DATA_ID && m_sHeader.usDataCount == entry.DATA_COUNT && m_sHeader.ucDataType == entry.DATA_TYPE;
        }

    private:
        const RoveCommPacketHeader m_sHeader;

        /*
         *  Shared pointer allows safe copying of packets without copying the data.
         *  This pointer should never be exposed directly.
         *
         *  Yes, data is stored on the heap, but the alternative is making EVERY packet occupy 65543 bytes.
         *  This is actually what Embedded RoveComm does (except the size changes between controllers).
         *  The Teensyduino compiler also allows variable sized stack allocated arrays.
         *
         *  Stack allocating the max amount would cause ALL 65543 BYTES to be copied on assignment. (BAD)
         *  In the vast majority of cases, we only send one or two ints, so we don't need 65 kilobytes.
         *
         *  Some packets could be big (camera data) but that will be fixed in the multithreading update.
         */
        const std::shared_ptr<char> m_pData;

    public:    ////////////////// STATIC METHODS /////////////////////////
        /******************************************************************************
         * @brief Helper function to write a RoveCommPacketHeader beginning at pDest.
         * This function does NOT allocate memory, so make sure pDest points to something!
         *
         * @param pDest - Where to begin writing.
         * @param header - Header to write.
         * @throw std::runtime_error if wrong version or data type.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-15
         ******************************************************************************/
        static void WriteHeader(char* pDest, const RoveCommPacketHeader& header);
        /******************************************************************************
         * @brief Helper function to read a RoveCommPacketHeader from an array beginning at pSource.
         *
         * @param dest - Existing RoveCommPacketHeader struct to set the values of.
         * @param pSource - Where to begin reading.
         * @throw std::runtime_error if wrong version or data type.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-15
         ******************************************************************************/
        static void ReadHeader(RoveCommPacketHeader& dest, const char* pSource);
        /******************************************************************************
         * @brief Helper function to wrute RoveCommPacket's data into an array beginning at pDest.
         * Converts from host byte order to network order.
         * This function does not also write the header.
         * This function does NOT allocate memory, so make sure pDest points to something!
         *
         * @param pDest - Where to begin writing.
         * @param packet - A description of the data to write.
         * @throw std::runtime_error if something went wrong.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-18
         ******************************************************************************/
        static void WriteData(char* pDest, const RoveCommPacket& packet);
        /******************************************************************************
         * @brief Helper function to read a RoveCommPacket's data from an array beginning at pSource.
         * Converts from network byte order to host order.
         * This function does not read the header.
         *
         * @param pDest - Where to begin writing result into.
         * @param pSource - Where to begin reading from.
         * @param header - Descriptor header for how to read data.
         * @throw std::runtime_error if something went wrong.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-18
         ******************************************************************************/
        static void ReadData(char* pDest, const char* pSource, const RoveCommPacketHeader& header);

        /******************************************************************************
         * @brief Helper function to allocate and fill an array with a RoveCommPacket's contents.
         * Calls WriteHeader() and WriteData() internally.
         *
         * @param packet - Packet to pack
         * @return RoveCommPacketBuffer - a struct containing { unique_ptr, size_t }
         * @throw std::runtime_error if something went wrong.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2024-01-19
         ******************************************************************************/
        static RoveCommPacketBuffer Pack(const RoveCommPacket& packet);
};

// print with std::cout
inline std::ostream& operator<<(std::ostream& out, const RoveCommPacket& packet);

#endif    // ROVECOMM_PACKET_H
