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
#include <future>
#include <iostream>
#include <sstream>
#include <string>

#include "RoveCommGlobals.h"

#include "RoveComm/RoveComm.h"

void StartServer()
{
    TCPServer().Start();
    // UDPServer().Start();
}

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

    // TODO: Initialize RoveComm
    std::thread th1(StartServer);

    th1.join();

    return 0;
}

#endif    // __ROVECOMM_LIBRARY_MODE__
