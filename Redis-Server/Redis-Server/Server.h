#pragma once

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

class Server
{
	private: 
        WSADATA wsApiData;

        SOCKET listenSocket = INVALID_SOCKET;
        SOCKET clientSocket = INVALID_SOCKET;

        struct addrinfo* result = NULL;
        struct addrinfo hints;

        int wsApiRes;
        int getAddrInfoRes;
        int bindRes;
        int listenRes;
        int sendDataRes;
        int shutDownRes;
        int receiveDataRes;

        int receiverBufferLength = DEFAULT_BUFLEN;
        char receiverBuffer[DEFAULT_BUFLEN];

	public:
        bool Initialize();
        bool Configure();
        bool Connect();
        bool Listen();
        bool SendReceiveData();
};