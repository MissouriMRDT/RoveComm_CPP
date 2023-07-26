/******************************************************************************
 * @brief Defines the NetworkAddress class.
 *
 * @file NetworkAddress.h
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

#include <string>

#ifndef NETWORKADDRESS_H
#define NETWORKADDRESS_H

enum NetworkAddressIntegers
{
    NAI_OCTET_ONE,
    NAI_OCTET_TWO,
    NAI_OCTET_THREE,
    NAI_OCTET_FOUR,
    NAI_PORT
};

class NetworkAddress
{
    public:
        NetworkAddress();
        ~NetworkAddress();
        NetworkAddress(std::string szIPAddress, int iPort);
        NetworkAddress(int iOctet1, int iOctet2, int iOctet3, int iOctet4, int iPort);

        int GetIData(NetworkAddressIntegers eKey);
        std::string GetSZData();

    private:
        int m_iOctet1;
        int m_iOctet2;
        int m_iOctet3;
        int m_iOctet4;
        int m_iPort;
};

#endif    // NETWORKADDRESS_H
