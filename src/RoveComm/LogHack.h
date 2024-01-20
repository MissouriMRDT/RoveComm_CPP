/******************************************************************************
 * @brief includes AutonomyLogging if in library mode, else RoveCommLogging
 *
 * @file LogHack.h
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2024-01-20
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#ifndef LOG_HACK_H
#define LOG_HACK_H

/******************************************************************************
 * @brief Since this project is setup as a library sometimes we need to access
 *        our logger and globals from the host application and othertimes, in
 *        the case that we are testing directly within our stand-alone
 *        application we need to access our logger and globals from within
 *        the repository but not necissarily from within the library itself.
 *
 *        The macro, __ROVECOMM_LIBRARY_MODE__, is how we can detect within
 *        RoveComm for High Level C++ Applications which mode we are running
 *        in and will tell us what can be included.
 *
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-08-28
 ******************************************************************************/
#ifdef __ROVECOMM_LIBRARY_MODE__
#include "../../../../../src/AutonomyConstants.h"
#include "../../../../../src/AutonomyGlobals.h"
#include "../../../../../src/AutonomyLogger.h"
#else
#include "../RoveCommGlobals.h"
#include "../RoveCommLogging.h"
#endif

#endif
