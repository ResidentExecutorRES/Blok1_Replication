#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include<stdlib.h>
#include<stdio.h>
#include <conio.h>
#include "../CommonLibrary/ClientData.h"
#include"../CommonLibrary/TCPFunctions.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 27016
#define ADDRESS "192.168.1.5"
#define SERVER_SLEEP_TIME 50

#pragma comment(lib, "Ws2_32.lib")


//127.0.0.1
// Initializes WinSock2 library
// Returns true if succeeded, false otherwise.


int main(int argc, char **argv)
{
	SOCKET connectSocket = INVALID_SOCKET;
	int iResult;

	if (InitializeWindowsSockets() == false)
	{
		return 1;
	}

	connectSocket = socket(AF_INET,
		SOCK_STREAM,
		IPPROTO_TCP);

	if (connectSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// create and initialize address structure
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
	serverAddress.sin_port = htons(DEFAULT_PORT);
	// connect to server specified in serverAddress and socket connectSocket


	if (connect(connectSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		printf("Unable to connect to server.\n");
		closesocket(connectSocket);
		WSACleanup();
	}

	unsigned long mode = 1; //nonblocking
	iResult = ioctlsocket(connectSocket, FIONBIO, &mode); //non

	if (iResult != NO_ERROR)
		printf("ioctlsocket failed with error: %dld\n", iResult);

	printf("Choose a staus: {1 - Open, 0 - Close}\n");
	char status;
	do {
		scanf("%c", &status);
		
	} while (status != '0' && status != '1');
	
	//int messageLength = 104857600; //100Mb
	char *messageToSend = Serialize(status);
	int messageLength = strlen(messageToSend);

	//iResult = send(connectSocket, messageToSend, (int)strlen(messageToSend) + 1, 0);
	iResult = Send(connectSocket, (char*)&messageLength, sizeof(int));
	Sleep(50);
	//char *messageToSend = (char*)malloc(messageLength);

	iResult = Send(connectSocket, messageToSend, messageLength);

	if (iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	getch();

	closesocket(connectSocket);
	WSACleanup();

	return 0;
}
