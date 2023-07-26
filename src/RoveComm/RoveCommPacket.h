/******************************************************************************
 * @brief Defines RoveCommPacket class and associated data types.
 *
 * @file RoveCommPacket.h
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-20
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

#include <string>

#ifndef ROVECOMMPACKET_H
#define ROVECOMMPACKET_H

enum DataTypes
{
    INT8_T,
    UINT8_T,
    INT16_T,
    UINT16_T,
    INT32_T,
    UINT32_T,
    FLOAT_T,
    DOUBLE_T,
    CHAR
};

template<typename T>
class RoveCommPacket
{
    public:
        RoveCommPacket();
        ~RoveCommPacket();

        void SetIPAddress(const std::string& szIPAddress, int iPort);
        std::string GetIPAddress();
        short unsigned int GetPort();
        T GetDataPacket();
        char GetCData(DataTypes eType);
        void Print();

    private:
        int m_iDataId{};
        DataTypes m_eDataType;
        T m_tData;
        int m_iDataCount{};
        NetworkAddress m_sIPAddress;
};

#endif    // ROVECOMMPACKET_H
