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


void Socket::ConfigureConnection()
{
	// initialize memory to zero for the storage of senstive data
	// Prevent undefined or garbage values that might be seen in the memory
	ZeroMemory(&this->hints, sizeof(this->hints));
	this->hints.ai_family = AF_INET;
	this->hints.ai_socktype = SOCK_STREAM;
	this->hints.ai_protocol = IPPROTO_TCP;
}


bool Socket::SetupAddress(char* argv[])
{
	getAddrInfoRes = getaddrinfo(argv[1], DEFAULT_PORT, &this->hints, &this->result);

	if (getAddrInfoRes != 0)
	{
		printf("Windows Socket Get Address Failed: %d\n", getAddrInfoRes);
		freeaddrinfo(result); // Frees address that getaddrinfo dynamically allocates.
		WSACleanup(); // Terminates the usage if Winsock2.
		
		return false;
	}

	return true;
}


bool Socket::ConfigureSocket()
{
	ptr = result;
	mainSocket = socket(this->ptr->ai_family, this->ptr->ai_socktype, this->ptr->ai_protocol);

	if (mainSocket == INVALID_SOCKET)
	{
		printf("Windows Socket Connection could not Established. (Error Code: %ld)", WSAGetLastError());
		freeaddrinfo(result); // Frees address that getaddrinfo dynamically allocates. Since we use 'result' that has dynamically allocated memory, call 'freeaddrinfo'.
		WSACleanup(); // Terminates the usage if Winsock2.

		return false;
	}

	return true;
}


bool Socket::Connect()
{
	connectRes = connect(mainSocket, this->ptr->ai_addr, (int)this->ptr->ai_addrlen);

	if (connectRes == SOCKET_ERROR)
	{
		closesocket(mainSocket);
		mainSocket = INVALID_SOCKET;

		/*
		// Try next address returned by getaddrinfo
		struct addrinfo* nextPtr = this->ptr->ai_next;
		connectRes = connect(mainSocket, nextPtr->ai_addr, (int)nextPtr->ai_addrlen);

		if (connectRes == SOCKET_ERROR)
		{
			closesocket(mainSocket);
			mainSocket = INVALID_SOCKET;
		}
		*/
	}

	if (mainSocket == INVALID_SOCKET)
	{
		printf("Unable to Connect to Server!\n");
		freeaddrinfo(result); // Frees address that getaddrinfo dynamically allocates.
		WSACleanup(); // Terminates the usage of Winsock2.

		return false;
	}

	return true;
}


bool Socket::SendReceiveData()
{
	const char* senderBuffer = "Hello, test client";
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
