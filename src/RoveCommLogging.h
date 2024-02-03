/******************************************************************************
 * @brief Implements Logging for Autonomy
 *
 *        Note: Functions and Variables are defined in AutonomyGlobals.cpp
 *              by having the declarations occur in a separate header we are
 *              more easily able to use the functionality of the logger in
 *              areas of the program that would normally be unaccessible
 *              due to it being included in the Main Globals Header.
 *
 * @file AutonomyLogging.hpp
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-11-11
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

// Only worry about compiling if we are not in 'Library-Only Mode'.
#ifndef __ROVECOMM_LIBRARY_MODE__

#include <chrono>
#include <ctime>
#include <iostream>
#include <quill/Quill.h>
#include <string>

#ifndef ROVECOMM_LOGGING_H
#define ROVECOMM_LOGGING_H

/******************************************************************************
 * @brief Logging Levels:
 *
 *        Priority > Level     > Description
 *        Level 1  > TRACE_L3  > Unused
 *        Level 2  > TRACE_L2  > Unused
 *        Level 3  > TRACE_L1  > Unused
 *        Level 4  > DEBUG     > Details that would only be useful in a debug environment
 *        Level 5  > INFO      > State Changes, RoveComm Updates GPS/IMU/Autonomy, etc
 *        Level 6  > WARNING   > Something unexpected happened - application could potentially error soon.
 *        Level 7  > ERROR     > Something went wrong - application could potentially have critical error soon.
 *        Level 8  > CRITICAL  > Something went very wrong - application will exit after logging is sent.
 *
 *        Note: At testing sessions we will have "DEBUG" Logging set as the level that is being outputted.
 *              However, at competition and when using "RELEASE" code we will be using "INFO" Logging. When
 *              a logging level is set, we only receive logging messages that are that level or higher
 *              priority.
 *
 *        Example: When INFO is set, we only receive: INFO, WARNING, ERROR, CRITICAL
 *                 When DEBUG is set, we only receive: DEBUG, INFO, WARNING, ERROR, CRITICAL
 *
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-08-22
 ******************************************************************************/

/******************************************************************************
 * @brief Quill logger instances for RoveComm to use when not in library mode
 *
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2024-01-24
 ******************************************************************************/
namespace logging
{
    // Logging:
    extern quill::Logger* g_qFileLogger;
    extern quill::Logger* g_qConsoleLogger;
    extern quill::Logger* g_qSharedLogger;

    void InitializeLoggers(std::string szLoggingOutputPath);
}    // namespace logging

#endif    // ROVECOMM_GLOBALS_H

#endif    // __ROVECOMM_LIBRARY_MODE__
