/******************************************************************************
 * @brief
 *
 * @file UDP.cpp
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-08-30
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#include "UDP.h"

/******************************************************************************
 * @brief Construct a new UDP::UDP object.
 *
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-08-30
 ******************************************************************************/
UDP::UDP()
{
    UDP(constants::UDP_PORT);
}

/******************************************************************************
 * @brief Construct a new UDP::UDP object.
 *
 * @param unPort -
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-08-30
 ******************************************************************************/
UDP::UDP(const unsigned int unPort)
{
    this->m_unPort = unPort;
    this->m_vSubscribers.clear();

    if ((m_nSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        LOG_ERROR(g_qSharedLogger, "Socket Creation Failed...");
        exit(EXIT_FAILURE);
    }
}

int UDP::Subscribe(AddressEntry stSubToIP) {}
