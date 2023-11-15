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


class Socket
{
	private:
		WSADATA wsApiData;

		struct addrinfo* result = nullptr;
		struct addrinfo* ptr = nullptr;
		struct addrinfo hints;

		SOCKET mainSocket = INVALID_SOCKET;

		int wsApiRes;
		int getAddrInfoRes;
		int connectRes;
		int shutDownRes;
		int receiveDataRes;

		int receiverBufferLength = DEFAULT_BUFLEN;
		char receiverBuffer[DEFAULT_BUFLEN];



	public:
		bool Initialize();
		bool ConfigureConnection(char* argv[]);
		bool Connect();
		bool SendReceiveData(const char* senderBuffer);
		
};