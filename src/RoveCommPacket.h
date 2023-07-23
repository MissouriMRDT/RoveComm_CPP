/******************************************************************************
 * @brief Defines RoveCommPacket class and associated data types.
 *
 * @file RoveCommPacket.h
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-20
 *
 * @copyright Copyright MRDT 2023 - All Rights Reserved
 ******************************************************************************/

#include "../../../src/AutonomyGlobals.h"
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
