/******************************************************************************
 * @brief Main program file. Sets up classes and runs main program functions.
 *
 * @file main.cpp
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-07-25
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

// Only worry about compiling if we are not in 'Library-Only Mode'.
#if !defined(__ROVECOMM_LIBRARY_MODE__) || (__ROVECOMM_LIBRARY_MODE__ == 0)

#include "RoveComm/RoveComm.h"
#include "RoveCommLoggers.h"

/// \cond
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

/// \endcond

using namespace rovecomm;

/******************************************************************************
 * @brief RoveComm C++ main function.
 *
 * @return int - Exit status number.
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-06-25
 ******************************************************************************/
int main()
{
    // Print Software Header
    std::ifstream fHeaderText("../data/ASCII/v3.txt");
    std::string szHeaderText;
    if (fHeaderText)
    {
        std::ostringstream pHeaderText;
        pHeaderText << fHeaderText.rdbuf();
        szHeaderText = pHeaderText.str();
    }

    std::cout << szHeaderText << std::endl;
    std::cout << "Copyright \u00A9 2023 - Mars Rover Design Team\n" << std::endl;

    // Initialize Loggers
    InitializeLoggers();

    // Define a callback function for handling received TCP packets
    auto TCPCallback = [](const rovecomm::RoveCommPacket<uint8_t>& packet)
    {
        std::cout << "Received TCP packet with:" << std::endl;
        std::cout << "\tData ID: " << packet.unDataId << std::endl;
        std::cout << "\tData Count: " << packet.unDataCount << std::endl;
        std::cout << "\tData Type: " << packet.eDataType << std::endl;
        std::cout << "\tData: " << std::endl;

        for (auto data : packet.vData)
        {
            std::cout << "\t\t>>" << (int) data << std::endl;
        }
    };

    // Define a callback function for handling received packets
    auto UDPCallback = [](const RoveCommPacket<uint8_t>& packet, const sockaddr_in& addr)
    {
        (void) addr;

        std::cout << "Received UDP packet with:" << std::endl;
        std::cout << "\tData ID: " << packet.unDataId << std::endl;
        std::cout << "\tData Count: " << packet.unDataCount << std::endl;
        std::cout << "\tData Type: " << packet.eDataType << std::endl;
        std::cout << "\tData: " << std::endl;

        for (auto data : packet.vData)
        {
            std::cout << "\t\t>>" << (int) data << std::endl;
        }
    };

    // Create RoveComm Nodes
    RoveCommUDP pRoveCommUDP_Node;
    RoveCommTCP pRoveCommTCP_Node;

    // Initialize the UDP and TCP nodes
    if (pRoveCommUDP_Node.InitUDPSocket(11000))
    {
        std::cout << "UDP Node Initialized." << std::endl;
    }
    else
    {
        std::cout << "UDP Node Failed to Initialize." << std::endl;
    }

    if (pRoveCommTCP_Node.InitTCPSocket("127.0.0.1", 12000))
    {
        std::cout << "TCP Node Initialized." << std::endl;
    }
    else
    {
        std::cout << "TCP Node Failed to Initialize." << std::endl;
    }

    // Add the callback functions for UINT8_T data type to the UDP and TCP nodes
    pRoveCommUDP_Node.AddUDPCallback<uint8_t>(UDPCallback, 11000);
    pRoveCommUDP_Node.AddUDPCallback<uint8_t>(UDPCallback, 11000);
    pRoveCommTCP_Node.AddTCPCallback<uint8_t>(TCPCallback, 11000);
    pRoveCommTCP_Node.AddTCPCallback<uint8_t>(TCPCallback, 11000);

    // Create RoveCommPacket
    RoveCommPacket<uint8_t> stPacket;
    stPacket.unDataId    = manifest::Autonomy::COMMANDS.find("STARTAUTONOMY")->second.DATA_ID;
    stPacket.unDataCount = manifest::Autonomy::COMMANDS.find("STARTAUTONOMY")->second.DATA_COUNT;
    stPacket.eDataType   = manifest::Autonomy::COMMANDS.find("STARTAUTONOMY")->second.DATA_TYPE;
    stPacket.vData.push_back(200);

    // Send the packet to the localhost
    pRoveCommUDP_Node.SendUDPPacket<uint8_t>(stPacket, "127.0.0.1", 11000);
    pRoveCommTCP_Node.SendTCPPacket<uint8_t>(stPacket, "127.0.0.1", 12000);

    // Wait for packets to be processed.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Remove the callback functions for UINT8_T data type from the UDP and TCP nodes
    pRoveCommUDP_Node.RemoveUDPCallback<uint8_t>(UDPCallback);
    pRoveCommUDP_Node.AddUDPCallback<uint8_t>(UDPCallback, 11000);

    // Send the packet to the localhost
    pRoveCommUDP_Node.SendUDPPacket<uint8_t>(stPacket, "127.0.0.1", 11000);
    pRoveCommTCP_Node.SendTCPPacket<uint8_t>(stPacket, "127.0.0.1", 12000);

    // Wait for packets to be processed.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    exit(0);
}

#endif    // __ROVECOMM_LIBRARY_MODE__
