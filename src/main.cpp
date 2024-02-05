#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

#include "RoveComm/RoveComm.h"
#include "RoveCommGlobals.h"

// Global mutex for synchronization
std::mutex gMutex;

/******************************************************************************
 * @brief RoveComm C++ main function.
 *
 * @return int - Exit status number.
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-06-25
 ******************************************************************************/
int main()
{
    // Print Software Header
    std::ifstream fHeaderText("../data/ASCII/v3.txt");
    std::string szHeaderText;
    if (fHeaderText)
    {
        std::ostringstream pHeaderText;
        pHeaderText << fHeaderText.rdbuf();
        szHeaderText = pHeaderText.str();
    }

    std::cout << szHeaderText << std::endl;
    std::cout << "Copyright \u00A9 2023 - Mars Rover Design Team\n" << std::endl;

    // Initialize Loggers
    InitializeLoggers();

    // Initialize RoveComm
    RoveComm roveComm;

    // Initialize and set up the UDP socket
    if (roveComm.initUDPSocket("127.0.0.1", 5000))
    {
        std::cout << "UDP socket initialized successfully\n";

        // Add UDP callback function
        roveComm.addUDPCallback(
            [](const RoveCommPacket& packet, const sockaddr_in& clientAddr)
            {
                (void) clientAddr;    // Suppress "unused parameter" warning
                std::cout << "Received UDP packet with Data ID: " << packet.data_id << std::endl;
                // Add your processing logic here
            },
            1001);    // Change '1' to the desired condition

        // Start a new thread for sending UDP packets
        std::thread senderThread(
            [&]()
            {
                while (true)
                {
                    {
                        std::lock_guard<std::mutex> lock(gMutex);    // Lock the mutex
                        // Perform any necessary setup or data preparation before sending
                        roveComm.sendUDPPacket(RoveCommPacket(1001, 1, manifest::DataTypes::UINT8_T, {14}), "127.0.0.1", 5000);
                    }
                    std::this_thread::sleep_for(std::chrono::seconds(1));    // Adjust the sleep duration
                }
            });

        // Main thread: Receive UDP packets and invoke callbacks
        while (true)
        {
            roveComm.receiveUDPPacketAndCallback();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));    // Add a small delay to prevent busy-waiting
        }

        // Join the sender thread to the main thread
        senderThread.join();
    }
    else
    {
        std::cerr << "Failed to initialize UDP socket\n";
        return 1;
    }

    // Note: You may similarly set up the TCP socket and callbacks.

    return 0;
}
