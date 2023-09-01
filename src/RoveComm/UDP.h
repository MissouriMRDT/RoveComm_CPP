/******************************************************************************
 * @brief
 *
 * @file UDP.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-08-30
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef UDP_H
#define UDP_H

#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "Consts.h"

#include "Manifest.h"
using namespace manifest;

#include "Packet.h"

/******************************************************************************
 * @brief Since this project is setup as a library sometimes we need to access
 *        our logger and globals from the host application and othertimes, in
 *        the case that we are testing directly within our stand-alone
 *        application we need to access our logger and globals from within
 *        the repository but not necissarily from within the library itself.
 *
 *        The macro, __ROVECOMM_LIBRARY_MODE__, is how we can detect within
 *        RoveComm for High Level C++ Applications which mode we are running
 *        in and will tell us what can be included.
 *
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-08-28
 ******************************************************************************/
#ifdef __ROVECOMM_LIBRARY_MODE__
#include "../../../../../src/AutonomyGlobals.h"
#include "../../../../../src/AutonomyLogger.h"
#else
#include "../RoveCommGlobals.h"
#endif

class UDP
{
    private:
        unsigned int m_unPort;
        std::vector<AddressEntry> m_vSubscribers;
        int m_nSocket;

    public:
        UDP();
        UDP(const unsigned int unPort);

        int Subscribe(AddressEntry stSubToIP);
        int Write(DataPacket stPacket);
        DataPacket Read();
        int Close();

        ~UDP();
}

#endif    // UDP_H
