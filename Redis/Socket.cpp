#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#include "Socket.h"

bool Socket::Initialize()
{
	wsApiRes = WSAStartup(MAKEWORD(2, 2), &this->wsApiData);

	if (wsApiRes != 0)
	{
		printf("Windows Socket Api Startup Failed: %d\n", wsApiRes);
		return false;
	}

	return true;
}


bool Socket::ConfigureConnection(char* argv[])
{
	// initialize memory to zero for the storage of senstive data
	// Prevent undefined or garbage values that might be seen in the memory
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getAddrInfoRes = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);

	if (getAddrInfoRes != 0)
	{
		printf("Windows Socket Get Address Failed: %d\n", getAddrInfoRes);
		WSACleanup(); // Terminates the usage if Winsock2.

		return false;
	}

	return true;
}

bool Socket::Connect()
{
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		mainSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (mainSocket == INVALID_SOCKET) 
		{
			printf("Socket Connection Failed with Error: %ld\n", WSAGetLastError());
			WSACleanup();

			return false;
		}

		// Connect to server.
		connectRes = connect(mainSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

		if (connectRes == SOCKET_ERROR) 
		{
			closesocket(mainSocket);
			mainSocket = INVALID_SOCKET;

			continue;
		}

		break;
	}

	freeaddrinfo(result);

	if (mainSocket == INVALID_SOCKET)
	{
		printf("Unable to Connect to Server!\n");
		WSACleanup();

		return false;
	}

	return true;
}


bool Socket::SendReceiveData(const char* senderBuffer)
{
	//const char* senderBuffer = "Hello, test client";
	int sendMessageRes = send(mainSocket, senderBuffer, (int)strlen(senderBuffer), 0); // Send an initial buffer

	if (sendMessageRes == SOCKET_ERROR)
	{
		printf("Sending Message Failed %d\n", WSAGetLastError());
		closesocket(mainSocket);  // Completely terminates communication.
		WSACleanup(); // Terminates the usage of Winsock2.

		return false;
	}

	printf("Bytes Sent: %ld\n", sendMessageRes);


	// shutdown the connection for sending since no more data will be sent
	// the client can still use the ConnectSocket for receiving data.
	shutDownRes = shutdown(mainSocket, SD_SEND);

	if (shutDownRes == SOCKET_ERROR)
	{
		printf("ShutDown Failed: %d\n", WSAGetLastError());
		closesocket(mainSocket); // Completely terminates communication.
		WSACleanup(); // Terminates the usage of Winsock2.

		return false;
	}

	do // Receive data until the server closes the connection.
	{
		receiveDataRes = recv(mainSocket, receiverBuffer, receiverBufferLength, 0);

		if (receiveDataRes > 0)
		{
			printf("Bytes Received: %d\n", receiveDataRes);
		}
		else if (receiveDataRes == 0)
		{
			printf("Connection Closed!\n");
		}
		else
		{
			printf("Receive Failed: %d\n", WSAGetLastError());
		}
	} while (receiveDataRes > 0);

	closesocket(mainSocket); // Completely terminates communication.
	WSACleanup(); // Terminates the usage of Winsock2.
}

#endif
