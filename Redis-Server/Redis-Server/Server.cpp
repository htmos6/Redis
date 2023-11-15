#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#include "Server.h"

bool Server::Initialize()
{
    // Initialize the Windows Socket API
    wsApiRes = WSAStartup(MAKEWORD(2, 2), &wsApiData);

    if (wsApiRes != 0)
    {
        printf("(Server) Failed to initialize Windows Socket API. Error code: %d\n", wsApiRes);
        return false;
    }

    return true;
}


bool Server::Configure()
{
    // Initialize memory to zero for sensitive data storage
    // Prevent undefined or garbage values in memory
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    getAddrInfoRes = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    if (getAddrInfoRes != 0)
    {
        printf("(Server) Failed to get address information. Error code: %d\n", getAddrInfoRes);
        WSACleanup();

        return false;
    }

    return true;
}


bool Server::Connect()
{
    // Create a SOCKET for the server to listen for client connections
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (listenSocket == INVALID_SOCKET)
    {
        printf("(Server) Failed to create socket. Error code: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();

        return false;
    }

    // Setup the TCP listening socket
    bindRes = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (bindRes == SOCKET_ERROR)
    {
        printf("(Server) Failed to bind connection. Error code: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();

        return false;
    }

    freeaddrinfo(result);
    return true;
}


bool Server::Listen()
{
    // Start listening for incoming connections
    listenRes = listen(listenSocket, SOMAXCONN);

    if (listenRes == SOCKET_ERROR)
    {
        // Display an error message if listening for incoming connections fails
        printf("(Server) Failed to initiate listening for incoming connections. Error code: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();

        return false;
    }

    // Accept a client socket
    clientSocket = accept(listenSocket, NULL, NULL);

    if (clientSocket == INVALID_SOCKET) {
        // Display an error message if accepting the client socket fails
        printf("(Server) Failed to accept incoming connection. Error code: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();

        return false;
    }

    // No longer need the server socket
    closesocket(listenSocket);

    return true;
}


bool Server::SendReceiveData()
{
    do  // Receive until the peer shuts down the connection
    {
        receiveDataRes = recv(clientSocket, receiverBuffer, receiverBufferLength, 0);

        if (receiveDataRes > 0)
        {
            // Display the number of bytes received
            printf("(Server) Received %d bytes of data.\n", receiveDataRes);
            printf("(Server) Incoming Data: %s\n", receiverBuffer);

            // Echo the buffer back to the sender
            sendDataRes = send(clientSocket, receiverBuffer, (int)strlen(receiverBuffer), 0);

            if (sendDataRes == SOCKET_ERROR)
            {
                // Display an error message if sending data back fails
                printf("(Server) Failed to send a response. Error code: %d\n", WSAGetLastError());
                closesocket(clientSocket);
                WSACleanup();

                return false;
            }

            // Display the number of bytes sent
            printf("(Server) Sent %d bytes of data.\n", sendDataRes);
        }
        else if (sendDataRes == 0)
        {
            // Display a message if the connection is closing
            printf("(Server) Connection closing...\n");
        }
        else
        {
            // Display an error message if receiving data fails
            printf("(Server) Failed to receive data. Error code: %d\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();

            return false;
        }

    } while (receiveDataRes > 0);

    // Shutdown the connection since we're done
    shutDownRes = shutdown(clientSocket, SD_SEND);

    if (shutDownRes == SOCKET_ERROR)
    {
        // Display an error message if shutting down the connection fails
        printf("(Server) Failed to shut down the connection. Error code: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();

        return false;
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();

    return true;
}

#endif
