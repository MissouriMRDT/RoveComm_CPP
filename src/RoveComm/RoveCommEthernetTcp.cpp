/******************************************************************************
 * @brief RoveCommEthernetTcp Implementation for RoveComm C++ Library
 *
 * @file RoveCommEthernetTcp.cpp
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-10-08
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#include "RoveCommEthernetTcp.h"

RoveCommEthernetTcp::RoveCommEthernetTcp(std::string szHost, int nPort = ROVECOMM_TCP_PORT)
{
    return;

    /*
        self.open_sockets = {}
        self.incoming_sockets = {}
        self.buffers = defaultdict(list)

        # configure a TCP socket
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # Allows the socket address to be reused after being closed
        self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        # Fixes an error on linux with opening the socket again too soon
        try:
            self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
        except AttributeError:
            pass
        # bind the socket to the current machines local network IP by default (can be specified as well)
        self.server.bind((HOST, PORT))
        # accept up to 5 simulataneous connections, before we start discarding them
        self.server.listen(5)
    */
}

void RoveCommEthernetTcp::CloseSockets()
{
    return;
}

int RoveCommEthernetTcp::Write(RoveCommPacket& Packet)
{
    return 1;
}

int RoveCommEthernetTcp::Connect(std::string szAddress)
{
    return 1;
}

void RoveCommEthernetTcp::HandleIncomingConnection()
{
    return;
}

RoveCommPacket* RoveCommEthernetTcp::Read()    // needs to return pointer to array of RoveCommPackets
{
    RoveCommPacket aReturnValues[10];
    return aReturnValues;
}

// FIXME: Move RoveCommEthernetTcp Class Source Implementation to here!
