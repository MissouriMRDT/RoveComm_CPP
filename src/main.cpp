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
    using namespace std::literals::chrono_literals;
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
    std::cout << "Copyright \u00A9 2024 - Mars Rover Design Team\n" << std::endl;

    // Define a callback function for handling received TCP packets
    auto fnTCPCallback = [](const rovecomm::RoveCommPacket<uint8_t>& stPacket)
    {
        std::cout << "Received TCP packet with:" << std::endl;
        std::cout << "\tData ID: " << stPacket.unDataId << std::endl;
        std::cout << "\tData Count: " << stPacket.GetDataCount() << std::endl;
        std::cout << "\tData Type: " << stPacket.eDataType << std::endl;
        std::cout << "\tData: " << std::endl;

        for (auto data : stPacket.vData)
        {
            std::cout << "\t\t>>" << (int) data << std::endl;
        }
    };

    // Define a callback function for handling received packets
    auto fnUDPCallback = [](const RoveCommPacket<uint8_t>& stPacket)
    {
        std::cout << "Received UDP packet with:" << std::endl;
        std::cout << "\tData ID: " << stPacket.unDataId << std::endl;
        std::cout << "\tData Count: " << stPacket.GetDataCount() << std::endl;
        std::cout << "\tData Type: " << stPacket.eDataType << std::endl;
        std::cout << "\tData: " << std::endl;

        for (auto data : stPacket.vData)
        {
            std::cout << "\t\t>>" << (int) data << std::endl;
        }
    };

    // Create RoveComm Nodes
    RoveCommUDP pRoveCommUDP_Node;
    RoveCommTCP pRoveCommTCP_Node;

    // Initialize the UDP and TCP nodes
    if (pRoveCommUDP_Node.Init(11000))
    {
        std::cout << "UDP Node Initialized." << std::endl;
    }
    else
    {
        std::cout << "UDP Node Failed to Initialize." << std::endl;
    }

    if (pRoveCommTCP_Node.Init("127.0.0.1", 12000))
    {
        std::cout << "TCP Node Initialized." << std::endl;
    }
    else
    {
        std::cout << "TCP Node Failed to Initialize." << std::endl;
    }

    using manifest::Autonomy::Commands::STARTAUTONOMY;

    // Add the callback functions for UINT8_T data type to the UDP and TCP nodes
    pRoveCommUDP_Node.On<STARTAUTONOMY>(fnUDPCallback);
    // pRoveCommUDP_Node.On<STARTAUTONOMY>([](const auto& stPacket) { std::cout << "UDP Callback 2\n"; });
    pRoveCommTCP_Node.On<STARTAUTONOMY>(fnTCPCallback);
    // pRoveCommTCP_Node.On<STARTAUTONOMY>([](const auto& stPacket) { std::cout << "TCP Callback 2\n"; });

    // pRoveCommUDP_Node.Send<STARTAUTONOMY>({200}); // This would send to 192.168.3.100 by default

    // Send the packet to the localhost
    pRoveCommUDP_Node.Send<STARTAUTONOMY>({200}, "127.0.0.1", 11000);
    pRoveCommTCP_Node.Send<STARTAUTONOMY>({200}, "127.0.0.1", 12000);

    // Wait for packets to be processed.
    std::this_thread::sleep_for(500ms);

    // Remove the callback functions for UINT8_T data type from the UDP and TCP nodes
    pRoveCommUDP_Node.Clear<STARTAUTONOMY>();
    pRoveCommTCP_Node.Clear<STARTAUTONOMY>();

    // Another way to create packets
    auto stPacket = rovecomm::CreatePacket<STARTAUTONOMY>(uint8_t{255});

    // Send the packet to the localhost again; shouldn't see any output since we cleared the callbacks
    pRoveCommUDP_Node.Send(stPacket, "127.0.0.1", 11000);
    pRoveCommTCP_Node.Send(stPacket, "127.0.0.1", 12000);

    // Wait for packets to be processed.
    std::this_thread::sleep_for(500ms);

    // Open remote UDP node to subscribe to the local UDP node.
    RoveCommUDP pRemoteRoveCommUDP_Node;
    if (pRemoteRoveCommUDP_Node.Init(11003))
    {
        std::cout << "Remote UDP Node Initialized." << std::endl;
    }

    pRemoteRoveCommUDP_Node.Subscribe("127.0.0.1", 11000);

    // Wait for packets to be processed.
    std::this_thread::sleep_for(500ms);

    pRemoteRoveCommUDP_Node.On<STARTAUTONOMY>(
        [](const auto& stPacket)
        {
            (void) stPacket;
            std::cout << "Remote UDP Callback" << std::endl;
        });

    // Will send to subscribers, including the remote node
    pRoveCommUDP_Node.Send<STARTAUTONOMY>({200});

    // Wait for packets to be processed.
    std::this_thread::sleep_for(500ms);

    pRemoteRoveCommUDP_Node.Unsubscribe("127.0.0.1", 11000);

    // Wait for packets to be processed.
    std::this_thread::sleep_for(500ms);

    // Callback should not be invoked since we unsubscribed the remote node.
    pRoveCommUDP_Node.Send<STARTAUTONOMY>({200});

    // Wait for packets to be processed.
    std::this_thread::sleep_for(500ms);

    // Close the UDP and TCP sockets
    pRoveCommUDP_Node.Close();
    pRoveCommTCP_Node.Close();

    exit(0);
}

#endif    // __ROVECOMM_LIBRARY_MODE__
