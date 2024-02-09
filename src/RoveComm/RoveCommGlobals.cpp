/******************************************************************************
 * @brief The RoveComm Globals file contains the global variables for the RoveComm
 *        library. This includes the vectors of callbacks for each data type.
 *
 * @file RoveCommConsts.cpp
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-07
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#include "RoveCommGlobals.h"

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
     * @brief The RoveComm::UDP namespace contains all of the functionality for the
     *        RoveComm library's UDP functionality. This includes the vectors of
     *        callbacks for each data type.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    namespace udp
    {
        // The vectors of UDP callbacks for each data type.
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<uint8_t>&, const sockaddr_in&)>, unsigned int>> vUInt8Callbacks;
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<int8_t>&, const sockaddr_in&)>, unsigned int>> vInt8Callbacks;
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<uint16_t>&, const sockaddr_in&)>, unsigned int>> vUInt16Callbacks;
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<int16_t>&, const sockaddr_in&)>, unsigned int>> vInt16Callbacks;
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<uint32_t>&, const sockaddr_in&)>, unsigned int>> vUInt32Callbacks;
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<int32_t>&, const sockaddr_in&)>, unsigned int>> vInt32Callbacks;
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<float>&, const sockaddr_in&)>, unsigned int>> vFloatCallbacks;
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<double>&, const sockaddr_in&)>, unsigned int>> vDoubleCallbacks;
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<char>&, const sockaddr_in&)>, unsigned int>> vCharCallbacks;
    }    // namespace udp

    /******************************************************************************
     * @brief The RoveComm::TCP namespace contains all of the functionality for the
     *        RoveComm library's TCP functionality. This includes the vectors of
     *        callbacks for each data type.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    namespace tcp
    {
        // The vectors of TCP callbacks for each data type.
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<uint8_t>&)>, uint16_t>> vUInt8Callbacks;
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<int8_t>&)>, uint16_t>> vInt8Callbacks;
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<uint16_t>&)>, uint16_t>> vUInt16Callbacks;
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<int16_t>&)>, uint16_t>> vInt16Callbacks;
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<uint32_t>&)>, uint16_t>> vUInt32Callbacks;
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<int32_t>&)>, uint16_t>> vInt32Callbacks;
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<float>&)>, uint16_t>> vFloatCallbacks;
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<double>&)>, uint16_t>> vDoubleCallbacks;
        std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<char>&)>, uint16_t>> vCharCallbacks;
    }    // namespace tcp
}    // namespace rovecomm
