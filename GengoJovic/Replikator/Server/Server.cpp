#include <ws2tcpip.h>
#include<stdlib.h>
#include<stdio.h>



#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27016"
#define SERVER_SLEEP_TIME 50


#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996) 

bool InitializeWindowsSockets();
int Select(SOCKET socket, int send);
int Recv(SOCKET acceptedSocket, char *recvBuffer, int buffLength);

int  main(void)
{
	// Socket used for listening for new clients 
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

	//unsigned long mode = 1;
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


	iResult = listen(listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	iResult = Select(listenSocket, 0);
	if (iResult == -1)
	{
		fprintf(stderr, "select failed with error: %ld\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	acceptedSocket = accept(listenSocket, NULL, NULL);

	if (acceptedSocket == INVALID_SOCKET)
	{
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(acceptedSocket);
		WSACleanup();
		return 1;
	}

	//non blocking
	mode = 1;
	iResult = ioctlsocket(acceptedSocket, FIONBIO, &mode);
	if (iResult != NO_ERROR)
		printf("ioctlsocket failed with error: %dld\n", iResult);


	iResult = Select(acceptedSocket, 0);
	if (iResult == -1)
	{
		fprintf(stderr, "select failed with error: %ld\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	//if((flags = fcntl(sock_descriptor, F_GETFL, 0))<0)

	printf("Server initialized, waiting for clients.\n");
	int size;
	char* recvbuf;
	iResult = Recv(acceptedSocket, (char*)&size, sizeof(int));
	recvbuf = (char*)malloc(size);
	Sleep(50);

	if (iResult > 0)
	{
		//printf("Message received from client: %s.\n", (recvbuf));
	}

	else if (iResult == 0)
	{
		// connection was closed gracefully
		printf("Connection with client closed.\n");
		closesocket(acceptedSocket);
	}
	else
	{
		// there was an error during recv
		printf("recv failed with error: %d\n", WSAGetLastError());
		closesocket(acceptedSocket);
	}

	iResult = Recv(acceptedSocket, recvbuf, size);
	if (iResult > 0)
	{
		// printf("Message received from client: %s.\n", recvbuf);
	}

	else if (iResult == 0)
	{
		// connection was closed gracefully
		printf("Connection with client closed.\n");
		closesocket(acceptedSocket);
	}
	else
	{
		// there was an error during recv
		printf("recv failed with error: %d\n", WSAGetLastError());
		closesocket(acceptedSocket);
	}
	getchar();

	//printf("Bytes Receive: %ld\n", iResult);

	// shutdown the connection since we're done
	iResult = shutdown(acceptedSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(acceptedSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(listenSocket);
	closesocket(acceptedSocket);
	WSACleanup();

	return 0;
}

bool InitializeWindowsSockets()
{
	WSADATA wsaData;
	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

int Select(SOCKET socket, int send)
{
	FD_SET set;
	timeval timeVal;
	int iResult;

	do
	{
		FD_ZERO(&set);
		FD_SET(socket, &set);

		timeVal.tv_sec = 0;
		timeVal.tv_usec = 0;

		if (send == 1)
		{
			iResult = select(0, NULL, &set, NULL, &timeVal);
		}
		else
		{
			iResult = select(0, &set, NULL, NULL, &timeVal);
		}

		if (iResult == SOCKET_ERROR)
		{
			fprintf(stderr, "select failed with error: %ld\n", WSAGetLastError());
			return iResult;
		}

		if (iResult == 0)
		{
			Sleep(SERVER_SLEEP_TIME);
			continue;
		}
		else if (iResult > 0)
		{
			return iResult;
		}
		else
		{
			return -1;
		}

	} while (iResult == 0);

	return -1;
}
int Recv(SOCKET acceptedSocket, char *recvBuffer, int buffLength)
{
	int i = 0;
	char *pomBuffer = recvBuffer;
	int iResult;


	while (buffLength > 0)
	{
		iResult = Select(acceptedSocket, 0);
		if (iResult == -1)
		{
			fprintf(stderr, "select failed with error: %ld\n", WSAGetLastError());
			closesocket(acceptedSocket);
			WSACleanup();
			return iResult;
		}

		i = recv(acceptedSocket, pomBuffer, buffLength, 0);
		printf("Received %d\n", i);
		if (i < 1)
		{
			return -1;
		}
		pomBuffer += i;
		buffLength -= i;
	}

	printf("RECEIVED ALL");
	return i;
}
