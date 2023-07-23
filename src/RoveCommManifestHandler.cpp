/******************************************************************************
 * @brief Implements the RoveCommManifestHandler class.
 *
 * @file RoveCommManifestHandler.cpp
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-20
 *
 * @copyright Copyright MRDT 2023 - All Rights Reserved
 ******************************************************************************/

#include "RoveCommManifestHandler.h"

/******************************************************************************
 * @brief Construct a new Rove Comm Manifest Handler:: Rove Comm Manifest Handler object.
 *
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-20
 ******************************************************************************/
RoveCommManifestHandler::RoveCommManifestHandler() {}

/******************************************************************************
 * @brief Destroy the Rove Comm Manifest Handler:: Rove Comm Manifest Handler object.
 *
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-20
 ******************************************************************************/
RoveCommManifestHandler::~RoveCommManifestHandler() {}

/******************************************************************************
 * @brief Compiles a set of commands for configuring a board.
 *
 * @param eValue - Enumerator specifying the type of board. Enum defined in
 * 					RoveCommManifestHandler.h file.
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-20
 ******************************************************************************/
void RoveCommManifestHandler::SetupBoard(RoveCommManifestIdentifiers eValue)
{
    // Read RoveComm Manifest into a tree
    boost::property_tree::ptree pRoot;
    boost::property_tree::read_json(ROVECOMM_MANIFEST_FILENAME, pRoot);

    // Create Manifest Node
    boost::property_tree::ptree pManifestNode = pRoot.get_child("RovecommManifest");

    switch (eValue)
    {
        case RCMI_CORE:
        {
            // Create Core Nodes
            boost::property_tree::ptree pCoreNode     = pManifestNode.get_child("Core");
            boost::property_tree::ptree pCoreCommands = pCoreNode.get_child("Commands");

            // Identify Core Components to Save
            std::string szCoreCommandNames[3] = {"DriveLeftRight", "LEDRGB", "StateDisplay"};

            // Save Core Board IP Address
            pCoreBoard.szIPAddress = pCoreNode.get<std::string>("Ip");

            // Save Core Board Commands
            for (auto& szCoreCommandName : szCoreCommandNames)
            {
                pCoreBoard.vCommands.push_back({szCoreCommandName,
                                                pCoreCommands.get_child(szCoreCommandName).get<int>("dataId"),
                                                pCoreCommands.get_child(szCoreCommandName).get<std::string>("dataType"),
                                                pCoreCommands.get_child(szCoreCommandName).get<int>("dataCount")});
            }

            break;
        }
        case RCMI_NAV:
        {
            // Create Nav Nodes
            boost::property_tree::ptree pNavNode      = pManifestNode.get_child("Nav");
            boost::property_tree::ptree pNavTelemetry = pNavNode.get_child("Telemetry");

            // Identify Core Components to Save
            std::string szNavTelemetryNames[3] = {"GPSLatLon", "IMUData", "AccuracyData"};

            // Save Core Board IP Address
            pNavBoard.szIPAddress = pNavNode.get<std::string>("Ip");

            // Save Nav Board Telemetry
            for (auto& szNavTelemetryName : szNavTelemetryNames)
            {
                pNavBoard.vTelemetry.push_back({szNavTelemetryName,
                                                pNavTelemetry.get_child(szNavTelemetryName).get<int>("dataId"),
                                                pNavTelemetry.get_child(szNavTelemetryName).get<std::string>("dataType"),
                                                pNavTelemetry.get_child(szNavTelemetryName).get<int>("dataCount")});
            }

            break;
        }
        default:
        {
            break;
        }
    }
}

/******************************************************************************
 * @brief Retrieve IP address of board.
 *
 * @param eIdentifier - Enumerator identifying what board to get IP of.
 * @return std::string - String containing 4 octet IPV4 address of board.
 *
 * @author Eli Byrd (edbgkk@mst.edu), ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-20
 ******************************************************************************/
std::string RoveCommManifestHandler::GetIPAddress(RoveCommManifestIdentifiers eIdentifier) const
{
    std::string szIPAddress = "0.0.0.0";

    switch (eIdentifier)
    {
        case RCMI_CORE: szIPAddress = pCoreBoard.szIPAddress; break;
        case RCMI_NAV: szIPAddress = pNavBoard.szIPAddress; break;
        default: break;
    }

    return szIPAddress;
}
