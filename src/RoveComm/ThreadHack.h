/******************************************************************************
 * @brief includes AutonomyThread if in library mode, else a local copy
 *
 * @file LogHack.h
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2024-01-20
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#ifndef THREAD_HACK_H
#define THREAD_HACK_H

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
#include "../../../../../src/interfaces/AutonomyThread.hpp"
#else
#include "../AutonomyThread.hpp"
#endif

#endif    // THREAD_HACK_H
