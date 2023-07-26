/******************************************************************************
 * @brief Defines the RoveCommManifestHandler class.
 *
 * @file RoveCommManifestHandler.h
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-20
 *
 * @copyright Copyright MRDT 2023 - All Rights Reserved
 ******************************************************************************/

//
// 07/26/2023 - Eli Byrd (edbgkk@mst.edu) Note:
// Since this project is setup as a library and depends on some packages and setup
// that are defined in the global header files in the repositories that make use
// of this library. When the library is compiled in CMakeLists.txt, we define a
// macro that can be used as a lookup to determine if we are in library mode or if
// we are running the source only. We can then use this macro to determine which
// globals header to include.
//
#ifdef __ROVECOMM_LIB_MODE__
#include "../../../../../src/AutonomyGlobals.h"
#else    // __ROVECOMM_LIB_MODE__
#include "../RoveCommGlobals.h"
#endif    // __ROVECOMM_LIB_MODE__

#include <array>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>

#define ROVECOMM_MANIFEST_FILENAME "../Source/Autonomy/Networking/Manifest.json"

#ifndef ROVECOMMMANIFESTHANDLER_H
#define ROVECOMMMANIFESTHANDLER_H

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

#endif    // ROVECOMMMANIFESTHANDLER_H
