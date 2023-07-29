/******************************************************************************
 * @brief
 *
 * @file RoveCommGlobals.cpp
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-07-29
 *
 * @copyright Copyright MRDT 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef __ROVECOMM_LIBRARY_MODE__
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
#endif    // __ROVECOMM_LIBRARY_MODE__
