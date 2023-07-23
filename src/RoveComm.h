/******************************************************************************
 * @brief Defines the RoveComm class.
 *
 * @file RoveComm.h
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-19
 *
 * @copyright Copyright MRDT 2023 - All Rights Reserved
 ******************************************************************************/

// #include "RoveCommEthernetTCP.h"
#include "RoveCommEthernetUDP.h"
#include "RoveCommManifestHandler.h"
#include "RoveCommPacket.h"

#include <string>

#ifndef ROVECOMM_H
#define ROVECOMM_H

class RoveComm
{
	public:
		RoveComm();
		~RoveComm();

	private:
		RoveCommManifestHandler pManifestHandler;
};

#endif	  // ROVECOMM_H
