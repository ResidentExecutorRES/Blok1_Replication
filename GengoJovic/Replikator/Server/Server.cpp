#include <ws2tcpip.h>
#include<stdlib.h>
#include<stdio.h>
#include "../CommonLibrary/ClientData.h"
#include "../CommonLibrary/CircularBuffer.h"
#include "../CommonLibrary/Threads.h"
#include "../CommonLibrary/TCPFunctions.h"


#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996) 

CRITICAL_SECTION cs;

int  main(void)
{
	SOCKET listenSocket = INVALID_SOCKET;
	// Socket used for communication with client
	SOCKET acceptedSocket = INVALID_SOCKET;
	// variable used to store function return value
	int iResult;


	// Buffer used for storing incoming data
	unsigned long mode = 1;

	//	int flags;

	if (InitializeWindowsSockets() == false)
	{
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
	}

	// Prepare address information structures
	addrinfo *resultingAddress = NULL;
	addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;       // IPv4 address
	hints.ai_socktype = SOCK_STREAM; // Provide reliable data streaming
	hints.ai_protocol = IPPROTO_TCP; // Use TCP protocol
	hints.ai_flags = AI_PASSIVE;     // 

									 // Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &resultingAddress);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	//nad listenSocketom radim readSet
	listenSocket = socket(AF_INET,      // IPv4 address famly
		SOCK_STREAM,  // stream socket
		IPPROTO_TCP); // TCP

	if (listenSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(resultingAddress);
		WSACleanup();
		return 1;
	}

	printf("Server is waiting...");

	iResult = ioctlsocket(listenSocket, FIONBIO, &mode);
	if (iResult != NO_ERROR)
		printf("ioctlsocket failed with error: %dld\n", iResult);

	// Setup the TCP listening socket - bind port number and local address 
	// to socket
	iResult = bind(listenSocket, resultingAddress->ai_addr, (int)resultingAddress->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(resultingAddress);
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	// Since we don't need resultingAddress any more, free it
	freeaddrinfo(resultingAddress);

	CBuffer *cBuffer;
	InitializeBuffer(&cBuffer, MIN_BUFF_SIZE);

	HANDLE listenThread = CreateThread(NULL, 0, &ListenClients, &listenSocket, 0, 0);

	do {
		sockaddr_in clientInfo;
		int sizeOfClientInfo = sizeof(clientInfo);
		int iResult = Select(listenSocket, 0);
		if (iResult == -1)
		{
			fprintf(stderr, "select failed with error: %ld\n", WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}

		acceptedSocket = accept(listenSocket, (sockaddr *)&clientInfo, &sizeOfClientInfo);
		if (acceptedSocket == INVALID_SOCKET)
		{
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(acceptedSocket);
			WSACleanup();
			return 1;
		}

		printf("\nClient connected: %s port: %d\n", inet_ntoa(clientInfo.sin_addr), clientInfo.sin_port);

		
		Params params = *(Params*)malloc(sizeof(Params));
		params.acceptSocket = acceptedSocket;
		params.listenSocket = listenSocket;
		params.socketInfo = clientInfo;
		params.cBuffer = &cBuffer;

	

		HANDLE retVal = CreateThread(NULL, 0, &AcceptClients, &params, 0, 0);


	} while (true);
		
	return 0;
}



