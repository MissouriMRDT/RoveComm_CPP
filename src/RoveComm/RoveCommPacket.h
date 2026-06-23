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

#include "./RoveCommConsts.h"
#include "./RoveCommManifest.h"

/// \cond
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#if defined(__ROVECOMM_WINDOWS_MODE__) && __ROVECOMM_WINDOWS_MODE__ == 1
#if !defined(ssize_t) && defined(_MSC_VER)
typedef int ssize_t;
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#elif _WIN32_WINNT < 0x0600
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#define _WINSOCKAPI_
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

/// \endcond

// These are used to convert 64-bit doubles to network bit order and back.
#define htonll(x) ((1 == htonl(1)) ? (x) : (((uint64_t) htonl((x) & 0xFFFFFFFFUL)) << 32) | htonl((uint32_t) ((x) >> 32)))
#define ntohll(x) ((1 == ntohl(1)) ? (x) : (((uint64_t) ntohl((x) & 0xFFFFFFFFUL)) << 32) | ntohl((uint32_t) ((x) >> 32)))

/******************************************************************************
 * @brief The RoveComm namespace contains all of the functionality for the
 *        RoveComm library. This includes the packet structure and the
 *        functions for packing and unpacking data.
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-07
 ******************************************************************************/
namespace rovecomm
{
    /******************************************************************************
     * @brief The RoveCommPacket struct is used to store data in a packet format
     *        for transmission over a network. This struct is used to store the
     *        data ID, data count, data type, and the data itself. The data is
     *        stored in a vector of the specified data type.
     *
     * @tparam T - The data type of the data to be stored in the packet.
     *             This can be any of the data types defined in the manifest.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    template<typename T>
    struct RoveCommPacket
    {
        public:
            uint16_t unDataId{};
            uint16_t unDataCount{};
            manifest::DataTypes eDataType{};
            std::vector<T> vData;
    };

    // RoveCommPacket and RoveCommData packing and unpacking functions
    template<typename T>
    std::vector<uint8_t> PackPacket(const RoveCommPacket<T>& stPacket);

    template<typename T>
    RoveCommPacket<T> UnpackData(std::span<const uint8_t> stData);

    // Packet creation utilities

    template<manifest::ManifestEntry Entry>
    using EntryType = typename manifest::Helpers::RoveCommToCType<Entry.DATA_TYPE>::c_type;

    template<manifest::ManifestEntry Entry>
    RoveCommPacket<EntryType<Entry>> CreatePacket()
    {
        RoveCommPacket<EntryType<Entry>> stPacket{.unDataId = Entry.DATA_ID, .unDataCount = Entry.DATA_COUNT, .eDataType = Entry.DATA_TYPE, .vData = {}};
        stPacket.vData.resize(stPacket.unDataCount);
        return stPacket;
    }

    template<manifest::ManifestEntry Entry, manifest::RoveCommType... Args>
    RoveCommPacket<EntryType<Entry>> CreatePacket(Args... args)
    {
        // Assume that the user expects the data count to be exactly correct since they are entering the arguments directly.
        // The runtime functions will extend or truncate the data as needed.
        static_assert(sizeof...(Args) == Entry.DATA_COUNT, "The number of arguments provided does not match the data count specified in the manifest entry.");
        return {.unDataId = Entry.DATA_ID, .unDataCount = Entry.DATA_COUNT, .eDataType = Entry.DATA_TYPE, .vData = {args...}};
    }

    template<manifest::ManifestEntry Entry>
    RoveCommPacket<EntryType<Entry>> CreatePacket(std::span<const EntryType<Entry>> spData)
    {
        RoveCommPacket<EntryType<Entry>> stPacket = {.unDataId    = Entry.DATA_ID,
                                                     .unDataCount = Entry.DATA_COUNT,
                                                     .eDataType   = Entry.DATA_TYPE,
                                                     .vData       = {spData.begin(), spData.end()}};
        stPacket.vData.resize(Entry.DATA_COUNT);
        return stPacket;
    }

    template<manifest::ManifestEntry Entry>
    RoveCommPacket<EntryType<Entry>> CreatePacket(std::initializer_list<EntryType<Entry>> ilData)
    {
        RoveCommPacket<EntryType<Entry>> stPacket = {.unDataId = Entry.DATA_ID, .unDataCount = Entry.DATA_COUNT, .eDataType = Entry.DATA_TYPE, .vData = ilData};
        stPacket.vData.resize(Entry.DATA_COUNT);
        return stPacket;
    }

}    // namespace rovecomm

#endif    // ROVECOMM_PACKET_H
