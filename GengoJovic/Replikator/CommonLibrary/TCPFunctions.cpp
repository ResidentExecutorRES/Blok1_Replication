#include"TCPFunctions.h"


bool InitializeWindowsSockets() {
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

	printf("\n");
	return i;
}

int Send(SOCKET connectSocket, char *messageToSend, int length)
{
	int i = 0;
	int sendLength = 0;
	char *pomBuffer = messageToSend;
	int iResult;


	while (sendLength < length)
	{
		iResult = Select(connectSocket, 1);
		if (iResult == -1)
		{
			fprintf(stderr, "select failed with error: %ld\n", WSAGetLastError());
			closesocket(connectSocket);
			WSACleanup();
			return 1;
		}

		i = send(connectSocket, pomBuffer, length - sendLength, 0);

		if (i < 1)
		{
			return -1;
		}
		pomBuffer += i;
		sendLength += i;
	}

	return i;
}
