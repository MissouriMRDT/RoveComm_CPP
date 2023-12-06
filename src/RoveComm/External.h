/******************************************************************************
 * @brief
 *
 * @file External.h
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-12-02
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_EXTERNAL_H
#define ROVECOMM_EXTERNAL_H

#ifdef __ROVECOMM_LIBRARY_MODE__

#else
// include path when not in library mode

#include "../AutonomyThread.hpp"
#include "../BSThreadPool.hpp"

#endif    // library mode

#endif    // ROVECOMM_EXTERNAL_H
