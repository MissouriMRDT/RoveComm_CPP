/******************************************************************************
 * @brief The RoveCommGlobals.h file is used to define functions and objects used
 *        at the global scope for the RoveComm library. This includes the
 *        SubscriberInfo struct and the vectors of callbacks for each data type.
 *
 * @file RoveCommGlobals.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-07
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_GLOBALS_H
#define ROVECOMM_GLOBALS_H

#include "./RoveCommPacket.h"

// \cond
#include <csignal>
#include <cstring>
#include <functional>
#include <iostream>
#include <vector>

/// \endcond

/******************************************************************************
 * @brief The RoveComm namespace contains all of the functionality for the
 *        RoveComm library. This includes the packet structure and the
 *        functions for packing and unpacking data.
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-07
 ******************************************************************************/
namespace rovecomm
{}        // namespace rovecomm

#endif    // ROVECOMM_GLOBALS_H
