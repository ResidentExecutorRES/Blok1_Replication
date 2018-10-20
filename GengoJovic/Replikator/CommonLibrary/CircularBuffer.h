#pragma once
//#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include"ClientData.h"

#define MAX_BUFF_SIZE 10240
#define MIN_BUFF_SIZE 10

typedef struct {
	ClientStatus *buffer;
	int pushIdx;
	int popIdx;
	int count;
	int bufferSize;
	CRITICAL_SECTION criticalSection;
} CBuffer;

void InitializeBuffer(CBuffer **buffer, int bufferSize);
void DestroyBuffer(CBuffer **buffer);

bool PushBuffer(CBuffer *buffer, ClientStatus value);
bool PopBuffer(CBuffer *buffer, ClientStatus *retVal);

void PrintBuffer(CBuffer *buffer);





