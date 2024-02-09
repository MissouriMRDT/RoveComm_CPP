/******************************************************************************
 * @brief Test Utilities
 *
 * @file TestUtils.cc
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-09
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#include "TestUtils.h"

/******************************************************************************
 * @brief The testutils namespace contains utility functions for running tests.
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-09
 ******************************************************************************/
namespace testutils
{
    /******************************************************************************
     * @brief Run a test with a timeout and retries.
     *
     * @param fnTestCode - The test code to run.
     * @param nMaxRetries - The maximum number of times to retry the test.
     * @param nTimeoutMS - The timeout for the test in milliseconds.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-09
     ******************************************************************************/
    void RunTimedTest(const std::function<void()>& fnTestCode, int nMaxRetries, int nTimeoutMS)
    {
        int nAttempts = 0;

        while (nAttempts < nMaxRetries)
        {
            std::atomic<bool> testComplete(false);

            // Start a new thread to run the test
            std::thread testThread(
                [&testComplete, fnTestCode]()
                {
                    // Run the test code
                    fnTestCode();
                    testComplete = true;
                });

            // Wait for the test to complete or timeout
            auto start   = std::chrono::steady_clock::now();

            bool bFailed = false;
            while (!testComplete)
            {
                auto now = std::chrono::steady_clock::now();
                if (now - start > std::chrono::milliseconds(nTimeoutMS))
                {
                    std::cerr << "Test timed out after " << nTimeoutMS << " ms" << std::endl;

                    testThread.detach();

                    // testThread.join();
                    bFailed = true;
                    break;
                }
            }

            if (testComplete && !bFailed)
            {
                testThread.join();
                break;
            }

            nAttempts++;
        }

        if (nAttempts >= nMaxRetries)
        {
            FAIL() << "Test failed after " << nMaxRetries << " attempts";
        }
    }
}    // namespace testutils
