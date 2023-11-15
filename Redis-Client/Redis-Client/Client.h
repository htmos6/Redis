#pragma once

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <iostream>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512


class Client
{
	private:
		WSADATA wsApiData;
		SOCKET mainSocket = INVALID_SOCKET;

		struct addrinfo* result = NULL;
		struct addrinfo* ptr = NULL;
		struct addrinfo hints;

		int wsApiRes;
		int getAddrInfoRes;
		int connectRes;
		int sendDataRes;
		int shutDownRes;
		int receiveDataRes;

		int receiverBufferLength = DEFAULT_BUFLEN;
		char receiverBuffer[DEFAULT_BUFLEN];

	public:
		bool Initialize();
		bool Configure(char* argv[]);
		bool Connect();
		bool SendReceiveData(const char* senderBuffer);
		
};