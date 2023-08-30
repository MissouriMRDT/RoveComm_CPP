/******************************************************************************
 * @brief
 *
 * @file TCP.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-08-30
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef TCP_H
#define TCP_H

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX  80
#define PORT 8080
#define SA   struct sockaddr

#include "Manifest.h"
using namespace manifest;

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

#include <thread>

class TCPClient : public std::thread
{
    public:
        void MessageParser(int sockfd);
        void Connect(AddressEntry stIP);
};

class TCPServer : public std::thread
{
    public:
        void MessageParser(int connfd);
        void Start();
};

#endif    // TCP_H
