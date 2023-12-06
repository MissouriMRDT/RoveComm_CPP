/******************************************************************************
 * @brief Network thread management
 *
 * @file RoveCommServer.cpp
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-13
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#include "RoveCommServer.h"

std::ostream& operator<<(std::ostream& out, const RoveCommAddress& address)
{
    out << address.m_cOctants[0] << '.';
    out << address.m_cOctants[1] << '.';
    out << address.m_cOctants[2] << '.';
    out << address.m_cOctants[3] << ':' << address.m_unPort;
    return out;
};

bool operator==(RoveCommAddress& address, RoveCommAddress& other)
{
    bool bSameIp = true;
    for (int i = 0; i < 4; i++)
    {
        if (address.m_cOctants[i] != other.m_cOctants[i])
        {
            bSameIp = false;
            break;
        }
    }
    return address.m_unPort == other.m_unPort && bSameIp;
}

bool operator!=(RoveCommAddress& address, RoveCommAddress& other)
{
    return !(address == other);
}
