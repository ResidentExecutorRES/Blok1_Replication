#pragma once

#include <ws2tcpip.h>
#include<stdlib.h>
#include<stdio.h>
#include"CircularBuffer.h"

#pragma comment(lib, "Ws2_32.lib")

typedef struct Params {
	SOCKET listenSocket;
	SOCKET acceptSocket;
	sockaddr_in socketInfo;
	CBuffer **cBuffer;
}Params;

typedef struct  BuffParam{
	CBuffer *cBuffer;
	ClientStatus clientStatus;
}BuffParam;

DWORD WINAPI ListenClients(LPVOID lpParam);

DWORD WINAPI AcceptClients(LPVOID lpParam);

DWORD WINAPI PushThread(LPVOID lpParam);

DWORD WINAPI PopThread(LPVOID lpParam);

void InitializeBufferParam(BuffParam *bufferParam);