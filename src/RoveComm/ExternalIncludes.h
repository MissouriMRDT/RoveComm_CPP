/******************************************************************************
 * @brief
 *
 * @file ExternalIncludes.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-07
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#ifndef EXTERNAL_INCLUDES_H
#define EXTERNAL_INCLUDES_H

// AutonomyThread.hpp is written by the Mars Rover Design Team and is included for
// our multithreading functionality.
#ifdef __ROVECOMM_LIBRARY_MODE__
#include "../../../../../src/interfaces/AutonomyThread.hpp"
#else
#include "../interfaces/AutonomyThread.hpp"
#endif

#endif    // EXTERNAL_INCLUDES_H
