/******************************************************************************
 * @brief Implements the RoveComm class.
 *
 * @file RoveComm.cpp
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-0619
 *
 * @copyright Copyright MRDT 2023 - All Rights Reserved
 ******************************************************************************/

#include "RoveComm.h"

/******************************************************************************
 * @brief Construct a new Rove Comm:: Rove Comm object.
 *
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-0619
 ******************************************************************************/
RoveComm::RoveComm()
{
	pManifestHandler = RoveCommManifestHandler();
	pManifestHandler.SetupBoard(RoveCommManifestIdentifiers::RCMI_CORE);
	pManifestHandler.SetupBoard(RoveCommManifestIdentifiers::RCMI_NAV);

	std::cout << pManifestHandler.GetIPAddress(RoveCommManifestIdentifiers::RCMI_CORE) << "\n"
			  << pManifestHandler.GetIPAddress(RoveCommManifestIdentifiers::RCMI_NAV) << std::endl;
}

/******************************************************************************
 * @brief Destroy the Rove Comm:: Rove Comm object.
 *
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-0619
 ******************************************************************************/
RoveComm::~RoveComm() {}
