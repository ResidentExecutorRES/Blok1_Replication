#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

typedef struct TList {
	sockaddr_in data;
	struct TList *next;
} TList;

TList* InitializeList();
void AddToList(TList **head, sockaddr_in data);
void RemoveFromList(TList **head, sockaddr_in data);

