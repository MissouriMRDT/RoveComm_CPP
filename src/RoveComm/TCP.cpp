/******************************************************************************
 * @brief
 *
 * @file TCP.cpp
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-08-30
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#include "TCP.h"

void TCPClient::MessageParser(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;)
    {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0)
        {
            printf("Client Exit...\n");
            break;
        }
    }
}

void TCPClient::Connect(AddressEntry stIP)
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    std::string szIP;

    szIP = "172.17.0.2";

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        LOG_ERROR(g_qSharedLogger, "Socket Creation Failed...");
        exit(0);
    }
    else
    {
        LOG_INFO(g_qSharedLogger, "Socket Creation Successful...");
    }
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(szIP.c_str());
    servaddr.sin_port        = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*) &servaddr, sizeof(servaddr)) != 0)
    {
        LOG_ERROR(g_qSharedLogger, "Server Connection Failed...");
        exit(0);
    }
    else
    {
        LOG_INFO(g_qSharedLogger, "Socket Connection Successful...");
    }

    // function for chat
    MessageParser(sockfd);

    // close the socket
    close(sockfd);
}

void TCPServer::MessageParser(int connfd)
{
    char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;)
    {
        bzero(buff, MAX);

        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        // print buffer which contains the client contents
        printf("From client: %s\t To client : ", buff);
        bzero(buff, MAX);
        n = 0;
        // copy server message in the buffer
        while ((buff[n++] = getchar()) != '\n')
            ;

        // and send that buffer to client
        write(connfd, buff, sizeof(buff));

        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0)
        {
            printf("Server Exit...\n");
            break;
        }
    }
}

void TCPServer::Start()
{
    unsigned int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        LOG_ERROR(g_qSharedLogger, "Socket Creation Failed...");
        exit(0);
    }
    else
        LOG_INFO(g_qSharedLogger, "Socket Creation Successful...");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*) &servaddr, sizeof(servaddr))) != 0)
    {
        LOG_ERROR(g_qSharedLogger, "Socket Bind Failed...");
        exit(0);
    }
    else
        LOG_INFO(g_qSharedLogger, "Socket Bind Successful...");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0)
    {
        LOG_ERROR(g_qSharedLogger, "Server Listening Failed...");
        exit(0);
    }
    else
        LOG_INFO(g_qSharedLogger, "Server Listening...");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*) &cli, &len);
    if (connfd < 0)
    {
        LOG_ERROR(g_qSharedLogger, "Server Accept Failed...");
        exit(0);
    }
    else
        LOG_INFO(g_qSharedLogger, "Server Accept Successful...");

    // Function for chatting between client and server
    MessageParser(connfd);

    // After chatting close the socket
    close(sockfd);
}
