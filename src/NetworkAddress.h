/******************************************************************************
 * @brief Defines the NetworkAddress class.
 *
 * @file NetworkAddress.h
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-0619
 *
 * @copyright Copyright MRDT 2023 - All Rights Reserved
 ******************************************************************************/
#include <string>

#ifndef NETWORKADDRESS_H
#	define NETWORKADDRESS_H

enum NetworkAddressIntegers
{
	NAI_OCTET_ONE,
	NAI_OCTET_TWO,
	NAI_OCTET_THREE,
	NAI_OCTET_FOUR,
	NAI_PORT
};

class NetworkAddress
{
	public:
		NetworkAddress();
		~NetworkAddress();
		NetworkAddress(std::string szIPAddress, int iPort);
		NetworkAddress(int iOctet1, int iOctet2, int iOctet3, int iOctet4, int iPort);

		int GetIData(NetworkAddressIntegers eKey);
		std::string GetSZData();

	private:
		int m_iOctet1;
		int m_iOctet2;
		int m_iOctet3;
		int m_iOctet4;
		int m_iPort;
};

#endif	  // NETWORKADDRESS_H
