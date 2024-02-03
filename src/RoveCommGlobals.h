/******************************************************************************
 * @brief Defines functions and objects used at the global scope for when
 *        testing the functionality of RoveComm C++.
 *
 *        Note: This is not compiled or packaged into the library that is
 *              distributed into other projects.
 *
 *
 * @file RoveCommGlobals.cpp
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-07-29
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

// Only worry about compiling if we are not in 'Library-Only Mode'.
#ifndef __ROVECOMM_LIBRARY_MODE__

#ifndef ROVECOMM_GLOBALS_H
#define ROVECOMM_GLOBALS_H
#include "RoveComm/RoveComm.h"

/******************************************************************************
 * @brief Global variables used when RoveComm is not in library mode
 *
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2024-01-24
 ******************************************************************************/
namespace globals
{
    extern RoveCommServerManager& RoveComm;
}

#endif    // ROVECOMM_GLOBALS_H

#endif    // __ROVECOMM_LIBRARY_MODE__
