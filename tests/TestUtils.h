/******************************************************************************
 * @brief Test Utilities
 *
 * @file TestUtils.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-09
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_TESTUTILS_H
#define ROVECOMM_TESTUTILS_H

#include <chrono>
#include <condition_variable>
#include <functional>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

/******************************************************************************
 * @brief The testutils namespace contains utility functions for running tests.
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-09
 ******************************************************************************/
namespace testutils
{
    // Run a test with a timeout and retries.
    extern void RunTimedTest(const std::function<void()>& fnTestCode, int nMaxRetries, int nTimeoutMS);
}    // namespace testutils

#endif    // ROVECOMM_TESTUTILS_H
