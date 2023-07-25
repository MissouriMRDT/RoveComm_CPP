/******************************************************************************
 * @brief
 *
 * @file RoveCommGlobals.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-07-25
 *
 * @copyright Copyright MRDT 2023 - All Rights Reserved
 ******************************************************************************/

#ifdef __LIBRARY_MODE__
#include "../../../../../src/AutonomyGlobals.h"
#else    // __LIBRARY_MODE__
#include <chrono>
#include <ctime>
#include <iostream>
#include <quill/Quill.h>

#ifndef ROVECOMM_GLOBALS_H
#define ROVECOMM_GLOBALS_H

extern quill::Logger* g_qFileLogger;
extern quill::Logger* g_qConsoleLogger;
extern quill::Logger* g_qSharedLogger;

void InitializeLoggers();

#endif    // ROVECOMM_GLOBALS_H
#endif    // __LIBRARY_MODE__
