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
#include <string>

#include "RoveCommManifest.h"

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

inline RoveCommProtocol operator|(RoveCommProtocol protocol, RoveCommProtocol other)
{
    return static_cast<RoveCommProtocol>(static_cast<unsigned int>(protocol) | static_cast<unsigned int>(other));
}

// using RoveCommPort = unsigned short;

struct RoveCommIp
{
        char firstOctet, secondOctet, thirdOctet, fourthOctet;

        RoveCommIp(char firstOctet, char secondOctet, char thirdOctet, char fourthOctet) :
            firstOctet(firstOctet), secondOctet(secondOctet), thirdOctet(thirdOctet), fourthOctet(fourthOctet)
        {}

        RoveCommIp(rovecomm::AddressEntry entry) : RoveCommIp(entry.FIRST_OCTET, entry.SECOND_OCTET, entry.THIRD_OCTET, entry.FOURTH_OCTET) {}

        std::string ToString() const;
        friend inline std::ostream& operator<<(std::ostream& out, const RoveCommIp& ip);
        friend inline bool operator==(const RoveCommIp& ip, const RoveCommIp& other);
        friend inline bool operator!=(const RoveCommIp& ip, const RoveCommIp& other);
};

inline std::ostream& operator<<(std::ostream& out, const RoveCommIp& ip);
inline bool operator==(const RoveCommIp& ip, const RoveCommIp& other);
inline bool operator!=(const RoveCommIp& ip, const RoveCommIp& other);

/******************************************************************************
 * @brief Contains the octets of an IPv4 address and a port number
 *
 * @param sOctets - the IPv4 address,
 * i.e. "1.2.3.4"
 * @param unPort - the port number
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-29
 ******************************************************************************/
class RoveCommAddress
{
    public:
        RoveCommAddress(RoveCommIp sOctets, uint16_t unPort) : m_sOctets(sOctets), m_unPort(unPort) {}

        RoveCommAddress(char cFirstOctet, char cSecondOctet, char cThirdOctet, char cFourthOctet, uint16_t unPort) :
            RoveCommAddress({cFirstOctet, cSecondOctet, cThirdOctet, cFourthOctet}, unPort)
        {}

        RoveCommAddress() : RoveCommAddress(0, 0, 0, 0, 0) {}

        // maybe have one that takes ("1.2.3.4", 0) ?

        inline RoveCommIp GetIp() const { return m_sOctets; }

        inline uint16_t GetPort() const { return m_unPort; }

        std::string ToString() const;
        friend inline std::ostream& operator<<(std::ostream& out, const RoveCommAddress& address);
        friend inline bool operator==(const RoveCommAddress& address, const RoveCommAddress& other);
        friend inline bool operator!=(const RoveCommAddress& address, const RoveCommAddress& other);

        // for std::map lookups
        friend inline bool operator<(const RoveCommAddress& address, const RoveCommAddress& other);

    private:
        RoveCommIp m_sOctets;
        uint16_t m_unPort;

    public:
        // pass to RoveCommServer::Fetch() signifying any address. This is not a valid address.
        const static RoveCommAddress ANY_ADDRESS;
};

// using RoveCommSocket = int;

inline std::ostream& operator<<(std::ostream& out, const RoveCommAddress& address);
inline bool operator==(const RoveCommAddress& address, const RoveCommAddress& other);
inline bool operator!=(const RoveCommAddress& address, const RoveCommAddress& other);

inline bool operator<(const RoveCommAddress& address, const RoveCommAddress& other)
{
    return address.m_unPort < other.m_unPort;
}

#endif
