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

#include "RoveComm/RoveCommConstants.h"
#include "RoveComm/RoveCommEthernetTcp.h"
#include "RoveComm/RoveCommEthernetUdp.h"
#include "RoveComm/RoveCommPacket.h"
#include "RoveCommGlobals.h"
#include "RoveCommLogging.h"

/******************************************************************************
 * @brief RoveComm C++ main function. Initializes singletons.
 *
 * @return int - Exit status number.
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-07-29
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
    logging::InitializeLoggers("./logs");

    globals::RoveComm.Init();

    // idk what to do here

    globals::RoveComm.Shutdown();
    return 0;
}

#endif    // __ROVECOMM_LIBRARY_MODE__
