/******************************************************************************
 * @brief All the boilerplate I didn't want to put in RoveCommServer.h
 *
 * @file RoveCommHelpers.h
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-12-21
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_HELPERS_H
#define ROVECOMM_HELPERS_H

#include <iostream>

/******************************************************************************
 * @brief Implemented network protocols. Currently only TCP/UDP.
 * In functions that take multiple protocols, write TCP | UDP.
 * To use all protocols, use RoveCommProtocol::ALL
 *
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-14
 ******************************************************************************/
enum RoveCommProtocol
{
    TCP = 1 << 0,
    UDP = 1 << 1,
    ALL = TCP | UDP
};

using RoveCommProtocolFlags = unsigned int;

inline RoveCommProtocolFlags operator|(RoveCommProtocol protocol, RoveCommProtocol other)
{
    return static_cast<RoveCommProtocolFlags>(static_cast<unsigned int>(protocol) | static_cast<unsigned int>(protocol));
}

using RoveCommPort = unsigned int;

using RoveCommIp   = struct
{
        char firstOctant, secondOctant, thirdOctant, fourthOctant;
        friend inline std::ostream& operator<<(std::ostream& out, const RoveCommIp& ip);
};

inline std::ostream& operator<<(std::ostream& out, const RoveCommIp& ip);
using RoveCommCallback = void();

/******************************************************************************
 * @brief Contains the octants of an IP address and a port number
 *
 * @param sOctants - the ipv4 address,
 * i.e. "1.2.3.4"
 * @param unPort - the port number
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-29
 ******************************************************************************/
class RoveCommAddress    // should this be a struct?
{
    public:
        RoveCommAddress(RoveCommIp sOctants, RoveCommPort unPort) : m_sOctants(sOctants), m_unPort(unPort) {}

        RoveCommAddress(char cFirstOctant, char cSecondOctant, char cThirdOctant, char cFourthOctant, RoveCommPort unPort) :
            RoveCommAddress({cFirstOctant, cSecondOctant, cThirdOctant, cFourthOctant}, unPort)
        {}

        RoveCommAddress() : RoveCommAddress(0, 0, 0, 0, 0) {}

        // maybe have one that takes ("1.2.3.4", 0) ?

        inline RoveCommIp GetIp() { return m_sOctants; }

        inline RoveCommPort GetPort() { return m_unPort; }

        friend inline std::ostream& operator<<(std::ostream& out, const RoveCommAddress& address);
        friend inline bool operator==(RoveCommAddress& address, RoveCommAddress& other);
        friend inline bool operator!=(RoveCommAddress& address, RoveCommAddress& other);

        // for std::map lookups
        inline bool operator<(RoveCommAddress& other) { return this->m_unPort < other.m_unPort; }

    private:
        RoveCommIp m_sOctants;
        RoveCommPort m_unPort;

    public:
        // pass to RoveCommServer::Fetch() signifying any address. This is not a valid address.
        const static RoveCommAddress ANY;
};

inline std::ostream& operator<<(std::ostream& out, const RoveCommAddress& address);
inline bool operator==(RoveCommAddress& address, RoveCommAddress& other);
inline bool operator!=(RoveCommAddress& address, RoveCommAddress& other);

#endif
