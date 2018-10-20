#include"Threads.h"
#include"TCPFunctions.h"
#include"ClientData.h"

DWORD WINAPI ListenClients(LPVOID lpParam) {
	SOCKET *listenSocket = (SOCKET*)lpParam;

	int iResult = listen(*listenSocket, SOMAXCONN);

	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(*listenSocket);
		WSACleanup();
		return 1;
	}
	return 0;
}

DWORD WINAPI AcceptClients(LPVOID lpParam) {
	// Socket used for listening for new clients 
	Params* params = (Params *)lpParam;
	SOCKET acceptedSocket = params->acceptSocket;
	SOCKET listenSocket = params->listenSocket;

	CBuffer **cBuffer = params->cBuffer;

	//non blocking
	unsigned long mode = 1;
	int iResult = ioctlsocket(acceptedSocket, FIONBIO, &mode);
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
		//printf("Message received from client: %s.\n", Deserialize(recv));
		ClientStatus clientStatus = Deserialize(recvbuf);
		PrintStruct(&clientStatus);
		
		if (PushBuffer(*cBuffer, clientStatus)) {
			printf("\nAdded...");
		}
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

	free(recvbuf);

	// cleanup
	closesocket(listenSocket);
	closesocket(acceptedSocket);
	WSACleanup();

	return 0;
}

DWORD WINAPI PushThread(LPVOID lpParam) {
	BuffParam **buffer = (BuffParam**)lpParam;
	
	if (PushBuffer((*buffer)->cBuffer, (*buffer)->clientStatus)) {
		printf("\nSuccessfully added to buffer!");
	}
	else {
		printf("\nSomething went wrong....");
	}
	return 0;
}

DWORD WINAPI PopThread(LPVOID lpParam) {
	BuffParam **buffer = (BuffParam**)lpParam;

	if (PopBuffer((*buffer)->cBuffer, &(*buffer)->clientStatus)) {
		printf("\nSuccessfully remove from buffer!");
	}
	else {
		printf("\nSomething went wrong....");
	}
	return 0;
}

void InitializeBufferParam(BuffParam *bufferParam) {
	 bufferParam = (BuffParam*)malloc(sizeof(BuffParam));
	 bufferParam->cBuffer = (CBuffer*)malloc(sizeof(CBuffer));
}