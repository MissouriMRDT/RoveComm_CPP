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
#ifndef __ROVECOMM_LIBRARY_MODE__

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "RoveComm/RoveComm.h"
#include "RoveCommGlobals.h"

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

    rovecomm::RoveCommTCP pRoveCommTCP;

    if (pRoveCommTCP.InitTCPSocket("127.0.0.1", 12000))
    {
        std::cout << "TCP Node Initialized." << std::endl;
        pRoveCommTCP.Start();

        rovecomm::RoveCommPacket<uint16_t> pTestPacket;
        pTestPacket.unDataId    = 11000;
        pTestPacket.unDataCount = 1;
        pTestPacket.eDataType   = manifest::DataTypes::UINT16_T;
        pTestPacket.vData.push_back(1);

        // pRoveCommTCP.AddTCPCallback<uint16_t>([](const rovecomm::RoveCommPacket<uint8_t>& pPacket, int nSocket)
        //                                      { std::cout << "Received Packet: " << pPacket.unDataId << std::endl; },
        //                                      11000);

        pRoveCommTCP.SendTCPPacket(0, pTestPacket);
    }
    else
    {
        std::cout << "TCP Node Failed to Initialize." << std::endl;
    }

    return 0;
}

#endif    // __ROVECOMM_LIBRARY_MODE__
