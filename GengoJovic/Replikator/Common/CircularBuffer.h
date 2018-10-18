#pragma once
//#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

#define MAX_BUFF_SIZE 10240
#define MIN_BUFF_SIZE 10


typedef struct {
	int *buffer;
	int pushIdx;
	int popIdx;
	int count;
	int bufferSize;
	CRITICAL_SECTION criticalSection;
} CBuffer;

void InitializeBuffer(CBuffer **buffer, int bufferSize);
void DestroyBuffer(CBuffer **buffer);

void ExpandBuffer(CBuffer *buffer);
void NarrowBuffer(CBuffer *buffer);

bool PushBuffer(CBuffer *buffer, int number);
bool PopBuffer(CBuffer *buffer, int *retVal);





