#include "RoveComm/RoveComm.h"
#include <iostream>
#include <unistd.h>    // For sleep function

int main()
{
    // Create an instance of RoveComm
    RoveComm roveComm;

    // Initialize the UDP socket with the server's IP address and port
    const char* serverIpAddress = "127.0.0.1";
    int udpPort                 = 11000;

    for (int i = 0; i < 3; ++i)    // Retry up to 3 times
    {
        if (!roveComm.initUDPSocket(serverIpAddress, udpPort))
        {
            std::cerr << "Error initializing UDP socket" << std::endl;
            if (i < 2)
            {
                std::cerr << "Retrying in 1 second..." << std::endl;
                sleep(1);    // Wait for 1 second before retrying
            }
            else
            {
                return 1;    // Maximum retries reached, exit with an error
            }
        }
        else
        {
            break;    // Success, exit the retry loop
        }
    }

    // Create a sample data packet
    uint16_t dataId              = 1001;
    uint16_t dataCount           = 1;
    manifest::DataTypes dataType = manifest::DataTypes::UINT8_T;
    std::vector<uint8_t> payload = {42};    // Sample payload data

    RoveCommPacket packetToSend(dataId, dataCount, dataType, payload);

    // Display information about the packet
    packetToSend.displayPacket();

    // Send the UDP packet
    ssize_t bytesSent = roveComm.sendUDPPacket(packetToSend, serverIpAddress, udpPort);

    if (bytesSent < 0)
    {
        std::cerr << "Error sending UDP packet" << std::endl;
    }
    else
    {
        std::cout << "Sent UDP packet successfully. Bytes sent: " << bytesSent << std::endl;
    }

    // Close the UDP socket
    roveComm.closeUDPSocket();

    return 0;
}
