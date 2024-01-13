#include "RoveCommHelpers.h"

#include <memory>

std::string RoveCommIp::ToString() const
{
    std::string ret = "";
    ret += firstOctet + '.' + secondOctet + '.' + thirdOctet + '.' + fourthOctet;
    return ret;
}

std::ostream& operator<<(std::ostream& out, const RoveCommIp& ip)
{
    out << ip.ToString();
}

bool operator==(RoveCommIp& ip, RoveCommIp& other)
{
    return ip.firstOctet == other.firstOctet && ip.secondOctet == other.secondOctet && ip.thirdOctet == other.thirdOctet && ip.fourthOctet == other.fourthOctet;
}

bool operator!=(RoveCommIp& ip, RoveCommIp& other)
{
    return !(ip == other);
}

std::string RoveCommAddress::ToString() const
{
    std::string ret = "";
    ret += m_sOctets.ToString() + ':';
    ret += m_unPort;
    return ret;
}

std::ostream& operator<<(std::ostream& out, const RoveCommAddress& address)
{
    out << address.m_sOctets << ':' << address.m_unPort;
    return out;
};

bool operator==(RoveCommAddress& address, RoveCommAddress& other)
{
    return address.m_unPort == other.m_unPort && address.m_sOctets == other.m_sOctets;
}

bool operator!=(RoveCommAddress& address, RoveCommAddress& other)
{
    return !(address == other);
}