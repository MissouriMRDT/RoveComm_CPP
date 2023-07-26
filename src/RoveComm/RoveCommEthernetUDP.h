/******************************************************************************
 * @brief Defines the RoveCommEthernetUDP class.
 *
 * @file RoveCommEthernetUDP.h
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 *
 * @copyright Copyright MRDT 2023 - All Rights Reserved
 ******************************************************************************/

//
// 07/26/2023 - Eli Byrd (edbgkk@mst.edu) Note:
// Since this project is setup as a library and depends on some packages and setup
// that are defined in the global header files in the repositories that make use
// of this library. When the library is compiled in CMakeLists.txt, we define a
// macro that can be used as a lookup to determine if we are in library mode or if
// we are running the source only. We can then use this macro to determine which
// globals header to include.
//
#ifdef __ROVECOMM_LIB_MODE__
#include "../../../../../src/AutonomyGlobals.h"
#else    // __ROVECOMM_LIB_MODE__
#include "../RoveCommGlobals.h"
#endif    // __ROVECOMM_LIB_MODE__

#include "NetworkAddress.h"

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#ifndef ROVECOMMETHERNETUDP_H
#define ROVECOMMETHERNETUDP_H

template<typename T>
class RoveCommEthernetUDP
{
    public:
        RoveCommEthernetUDP();
        ~RoveCommEthernetUDP();
        RoveCommEthernetUDP(int iPort);

        int Subscribe(NetworkAddress pSubscribeToIP);

        int Write(RoveCommPacket<int8_t> pPacket);
        int Write(RoveCommPacket<uint8_t> pPacket);
        int Write(RoveCommPacket<int16_t> pPacket);
        int Write(RoveCommPacket<uint16_t> pPacket);
        int Write(RoveCommPacket<int32_t> pPacket);
        int Write(RoveCommPacket<uint32_t> pPacket);
        int Write(RoveCommPacket<float_t> pPacket);
        int Write(RoveCommPacket<double_t> pPacket);
        int Write(RoveCommPacket<char> pPacket);

        RoveCommPacket<T> ReadPacket();

        void CloseSocket();

    private:
        NetworkAddress m_pNetworkAddress;
        // TODO: Subscribers
};

#endif    // ROVECOMMETHERNETUDP_H
