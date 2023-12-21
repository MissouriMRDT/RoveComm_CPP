#include "RoveCommHelpers.h"

std::ostream& operator<<(std::ostream& out, const RoveCommIp& ip)
{
    out << ip.firstOctant << ':' << ip.secondOctant << ':' << ip.thirdOctant << ':' << ip.fourthOctant;
    return out;
}

std::ostream& operator<<(std::ostream& out, const RoveCommAddress& address)
{
    out << address.m_sOctants << ':' << address.m_unPort;
    return out;
};

bool operator==(RoveCommAddress& address, RoveCommAddress& other)
{
    return address.m_unPort == other.m_unPort && address.m_sOctants.firstOctant == other.m_sOctants.firstOctant &&
           address.m_sOctants.secondOctant == other.m_sOctants.secondOctant && address.m_sOctants.thirdOctant == other.m_sOctants.thirdOctant &&
           address.m_sOctants.fourthOctant == other.m_sOctants.fourthOctant;
}

bool operator!=(RoveCommAddress& address, RoveCommAddress& other)
{
    return !(address == other);
}
