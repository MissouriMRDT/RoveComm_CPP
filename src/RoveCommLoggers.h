/******************************************************************************
 * @brief Defines functions and objects used at the global scope for when
 *        testing the functionality of RoveComm C++.
 *
 *        Note: This is not compiled or packaged into the library that is
 *              distributed into other projects.
 *
 *
 * @file RoveCommLoggers.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-07-29
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

// Only worry about compiling if we are not in 'Library-Only Mode'.
#ifndef __ROVECOMM_LIBRARY_MODE__

#include <chrono>
#include <ctime>
#include <iostream>
#include <quill/Quill.h>

#ifndef ROVECOMM_LOGGER_H
#define ROVECOMM_LOGGER_H

// Logging:
extern quill::Logger* g_qFileLogger;
extern quill::Logger* g_qConsoleLogger;
extern quill::Logger* g_qSharedLogger;

void InitializeLoggers();

#endif    // ROVECOMM_LOGGER_H

#endif    // __ROVECOMM_LIBRARY_MODE__
