/******************************************************************************
 * @brief Implements the RoveCommEthernetUDP class.
 *
 * @file RoveCommEthernetUDP.cpp
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 *
 * @copyright Copyright MRDT 2023 - All Rights Reserved
 ******************************************************************************/
#include "RoveCommEthernetUDP.h"

/******************************************************************************
 * @brief Construct a new Rove Comm Ethernet U D P< T>:: Rove Comm Ethernet U D P object.
 *
 * @tparam T - Template argument for data type
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 ******************************************************************************/
template<typename T>
RoveCommEthernetUDP<T>::RoveCommEthernetUDP()
{
	this->m_pNetworkAddress = {"192.168.1.166", 3303};
}

/******************************************************************************
 * @brief Destroy the Rove Comm Ethernet U D P< T>:: Rove Comm Ethernet U D P object.
 *
 * @tparam T -
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 ******************************************************************************/
template<typename T>
RoveCommEthernetUDP<T>::~RoveCommEthernetUDP()
{
}

/******************************************************************************
 * @brief Construct a new Rove Comm Ethernet U D P< T>:: Rove Comm Ethernet U D P object.
 *
 * @tparam T - Template argument for data type
 * @param iPort - UDP port
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 ******************************************************************************/
template<typename T>
RoveCommEthernetUDP<T>::RoveCommEthernetUDP(int iPort)
{
	this->m_pNetworkAddress = {"192.168.1.166", iPort};
}

/******************************************************************************
 * @brief Create a listener for a given IP.
 *
 * @tparam T - Template argument for data type
 * @param pSubscribeToIP - IP to subscribe to
 * @return int - Subscribe status
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 ******************************************************************************/
template<typename T>
int RoveCommEthernetUDP<T>::Subscribe(NetworkAddress pSubscribeToIP)
{
	return 0;
}

/******************************************************************************
 * @brief Write data to RoveComm with int8 packet.
 *
 * @tparam T - Template argument for data type
 * @param pPacket - RoveComm packet containing data to write
 * @return int - Write status
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 ******************************************************************************/
template<typename T>
int RoveCommEthernetUDP<T>::Write(RoveCommPacket<int8_t> pPacket)
{
	return 0;
}

/******************************************************************************
 * @brief Write data to RoveComm with uint8 packet.
 *
 * @tparam T - Template argument for data type
 * @param pPacket - RoveComm packet containing data to write
 * @return int - Write status
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 ******************************************************************************/
template<typename T>
int RoveCommEthernetUDP<T>::Write(RoveCommPacket<uint8_t> pPacket)
{
	return 0;
}

/******************************************************************************
 * @brief Write data to RoveComm with int16 packet.
 *
 * @tparam T - Template argument for data type
 * @param pPacket - RoveComm packet containing data to write
 * @return int - Write status
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 ******************************************************************************/
template<typename T>
int RoveCommEthernetUDP<T>::Write(RoveCommPacket<int16_t> pPacket)
{
	return 0;
}

/******************************************************************************
 * @brief Write data to RoveComm with uint16 packet.
 *
 * @tparam T - Template argument for data type
 * @param pPacket - RoveComm packet containing data to write
 * @return int - Write status
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 ******************************************************************************/
template<typename T>
int RoveCommEthernetUDP<T>::Write(RoveCommPacket<uint16_t> pPacket)
{
	return 0;
}

/******************************************************************************
 * @brief Write data to RoveComm with int32 packet.
 *
 * @tparam T - Template argument for data type
 * @param pPacket - RoveComm packet containing data to write
 * @return int - Write status
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 ******************************************************************************/
template<typename T>
int RoveCommEthernetUDP<T>::Write(RoveCommPacket<int32_t> pPacket)
{
	return 0;
}

/******************************************************************************
 * @brief Write data to RoveComm with uint32 packet.
 *
 * @tparam T - Template argument for data type
 * @param pPacket - RoveComm packet containing data to write
 * @return int - Write status
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 ******************************************************************************/
template<typename T>
int RoveCommEthernetUDP<T>::Write(RoveCommPacket<uint32_t> pPacket)
{
	return 0;
}

/******************************************************************************
 * @brief Write data to RoveComm with float packet.
 *
 * @tparam T - Template argument for data type
 * @param pPacket - RoveComm packet containing data to write
 * @return int - Write status
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 ******************************************************************************/
template<typename T>
int RoveCommEthernetUDP<T>::Write(RoveCommPacket<float_t> pPacket)
{
	return 0;
}

/******************************************************************************
 * @brief Write data to RoveComm with double packet.
 *
 * @tparam T - Template argument for data type
 * @param pPacket - RoveComm packet containing data to write
 * @return int - Write status
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 ******************************************************************************/
template<typename T>
int RoveCommEthernetUDP<T>::Write(RoveCommPacket<double_t> pPacket)
{
	try
	{
		boost::asio::io_context io_context;

		boost::asio::ip::udp::socket socket {io_context};
		socket.open(boost::asio::ip::udp::v4());

		socket.send_to(boost::asio::buffer(boost::lexical_cast<std::string>(pPacket.GetDataPacket())),
					   boost::asio::ip::udp::endpoint {boost::asio::ip::make_address(pPacket.GetIPAddress()), pPacket.GetPort()});

		printf("SEND PACKET!\n");
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}

	return 0;
}

/******************************************************************************
 * @brief Write data to RoveComm with char packet.
 *
 * @tparam T - Template argument for data type
 * @param pPacket - RoveComm packet containing data to write
 * @return int - Write status
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 ******************************************************************************/
template<typename T>
int RoveCommEthernetUDP<T>::Write(RoveCommPacket<char> pPacket)
{
	return 0;
}

/******************************************************************************
 * @brief Read packets sent to this RoveComm instance.
 *
 * @tparam T - Template argument for data type
 * @return RoveCommPacket<T> - Packet with data of template type
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 ******************************************************************************/
template<typename T>
RoveCommPacket<T> RoveCommEthernetUDP<T>::ReadPacket()
{
	return RoveCommPacket<T>();
}

/******************************************************************************
 * @brief Close a RoveComm instance connection.
 *
 * @tparam T - Template argument for data type
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 ******************************************************************************/
template<typename T>
void RoveCommEthernetUDP<T>::CloseSocket()
{
}
