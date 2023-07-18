/******************************************************************************
 * @brief Defines the RoveCommManifestHandler class.
 *
 * @file RoveCommManifestHandler.h
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 06-20-2023
 *
 * @copyright Copyright MRDT 2023 - All Rights Reserved
 ******************************************************************************/
#include <array>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>

#define ROVECOMM_MANIFEST_FILENAME "../Source/Autonomy/Networking/Manifest.json"

#ifndef ROVECOMMMANIFESTHANDLER_H
#	define ROVECOMMMANIFESTHANDLER_H

enum RoveCommManifestIdentifiers
{
	RCMI_CORE,
	RCMI_NAV
};

enum RoveCommManifestIntegers
{
	RCMI_DATA_ID,
	RCMI_DATA_COUNT
};

enum RoveCommManifestStrings
{
	RCMSZ_NAME,
	RCMSZ_DATA_TYPE
};

struct RoveCommBoardCommand
{
		std::string szName;
		int iDataId;
		std::string szDataType;
		int iDataCount;
};

struct RoveCommBoard
{
		std::string szIPAddress;
		std::vector<RoveCommBoardCommand> vCommands;
		std::vector<RoveCommBoardCommand> vTelemetry;
};

class RoveCommManifestHandler
{
	public:
		RoveCommManifestHandler();
		~RoveCommManifestHandler();

		void SetupBoard(RoveCommManifestIdentifiers eValue);

		std::string GetIPAddress(RoveCommManifestIdentifiers eIdentifier) const;

	private:
		RoveCommBoard pCoreBoard;
		RoveCommBoard pNavBoard;
};

#endif	  // ROVECOMMMANIFESTHANDLER_H
