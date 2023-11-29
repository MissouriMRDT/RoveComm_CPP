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
    out << address.cOctants[0] << '.';
    out << address.cOctants[1] << '.';
    out << address.cOctants[2] << '.';
    out << address.cOctants[3] << ':' << address.unPort;
    return out;
};