/******************************************************************************
 * @brief Implements the NetworkAddress class.
 *
 * @file NetworkAddress.cpp
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-0619
 *
 * @copyright Copyright MRDT 2023 - All Rights Reserved
 ******************************************************************************/
#include "NetworkAddress.h"

/******************************************************************************
 * @brief Construct a new Network Address:: Network Address object.
 *
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-0619
 ******************************************************************************/
NetworkAddress::NetworkAddress()
{
	// IP Address
	this->m_iOctet1 = 0;
	this->m_iOctet2 = 0;
	this->m_iOctet3 = 0;
	this->m_iOctet4 = 0;

	// Port
	this->m_iPort = 0;
}

/******************************************************************************
 * @brief Destroy the Network Address:: Network Address object.
 *
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-0619
 ******************************************************************************/
NetworkAddress::~NetworkAddress() {}

/******************************************************************************
 * @brief Construct a new Network Address:: Network Address object.
 *
 * @param szIPAddress - String containing IP address
 * @param iPort - Port of service associated with IP
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-0619
 ******************************************************************************/
NetworkAddress::NetworkAddress(std::string szIPAddress, int iPort)
{

	// Set the length of the IP Address String
	const unsigned long lIPAddressLength = szIPAddress.length();

	// Create a counter object for setting integer based IP Octets
	unsigned int iOctetCounter = 0;

	// Create Character Pointers for the substring process
	char* pIPAddress;
	char* pOctetOfIPAddress;

	// Create a new character array
	pIPAddress = new char[lIPAddressLength + 1];

	// Copy the passed in IP Address that was passed
	// into the function into the character array.
	strcpy(pIPAddress, szIPAddress.c_str());

	// Retrieve the first octet of the IP Address
	pOctetOfIPAddress = strtok(pIPAddress, ".");

	while (pOctetOfIPAddress != nullptr)
	{

		// Convert the octet character pointer into a string
		std::string szOctet = pOctetOfIPAddress;

		// Convert the string to an integer and assign it to the appropriate octet
		switch (iOctetCounter)
		{
			case 0: this->m_iOctet1 = std::stoi(szOctet); break;
			case 1: this->m_iOctet2 = std::stoi(szOctet); break;
			case 2: this->m_iOctet3 = std::stoi(szOctet); break;
			case 3: this->m_iOctet4 = std::stoi(szOctet); break;
			default: PLOG_FATAL_(AL_ConsoleLogger) << "Reached Out of Bounds Index"; break;
		}

		// Retrieve the next octet of the IP Address
		pOctetOfIPAddress = strtok(nullptr, "-");

		// Increment the Octet Counter
		iOctetCounter++;
	}

	// Set Port
	this->m_iPort = iPort;

	// Deinitialize Character Pointers
	delete pIPAddress;
	delete pOctetOfIPAddress;
}

/******************************************************************************
 * @brief Construct a new Network Address:: Network Address object.
 *
 * @param iOctet1 - First octet of IP address
 * @param iOctet2 - Second octet of IP address
 * @param iOctet3 - Third octet of IP address
 * @param iOctet4 - Four octet of IP address
 * @param iPort - Port of service associated with IP
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-0619
 ******************************************************************************/
NetworkAddress::NetworkAddress(int iOctet1, int iOctet2, int iOctet3, int iOctet4, int iPort)
{
	// IP Address
	this->m_iOctet1 = iOctet1;
	this->m_iOctet2 = iOctet2;
	this->m_iOctet3 = iOctet3;
	this->m_iOctet4 = iOctet4;

	// Port
	this->m_iPort = iPort;
}

/******************************************************************************
 * @brief
 *
 * @param eKey -
 * @return int -
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-0619
 ******************************************************************************/
int NetworkAddress::GetIData(NetworkAddressIntegers eKey)
{
	int iValue = 0;

	switch (eKey)
	{
		case NAI_OCTET_ONE: iValue = m_iOctet1; break;
		case NAI_OCTET_TWO: iValue = m_iOctet2; break;
		case NAI_OCTET_THREE: iValue = m_iOctet3; break;
		case NAI_OCTET_FOUR: iValue = m_iOctet4; break;
		case NAI_PORT: iValue = m_iPort; break;
		default: PLOG_FATAL_(AL_ConsoleLogger) << "Reached invalid Index!"; break;
	}

	return iValue;
}

/******************************************************************************
 * @brief Getter for IP address data.
 *
 * @return std::string - The assembled IP string.
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-0619
 ******************************************************************************/
std::string NetworkAddress::GetSZData()
{
	return std::to_string(m_iOctet1) + "." + std::to_string(m_iOctet2) + "." + std::to_string(m_iOctet3) + "." + std::to_string(m_iOctet4);
}
