#pragma once

#include <ws2tcpip.h>
#include<stdlib.h>
#include<stdio.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27016"

#define SERVER_SLEEP_TIME 50

bool InitializeWindowsSockets();

int Select(SOCKET socket, int send);

int Recv(SOCKET acceptedSocket, char *recvBuffer, int buffLength);

int Send(SOCKET connectSocket, char *messageToSend, int length);
