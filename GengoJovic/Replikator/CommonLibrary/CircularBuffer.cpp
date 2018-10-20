#include "CircularBuffer.h"
#include <stdio.h>

void InitializeBuffer(CBuffer **buffer, int bufferSize) {
	*buffer = (CBuffer*)malloc(sizeof(CBuffer));
	InitializeCriticalSection(&(*buffer)->criticalSection);
	(*buffer)->bufferSize = bufferSize;
	(*buffer)->buffer = (ClientStatus*)malloc(sizeof(ClientStatus)*bufferSize);
	(*buffer)->count = 0;
	(*buffer)->popIdx = 0;
	(*buffer)->pushIdx = 0;

}

void DestroyBuffer(CBuffer **buffer) {
	free((*buffer)->buffer);
	DeleteCriticalSection(&(*buffer)->criticalSection);
	free(*buffer);
}

bool PushBuffer(CBuffer *buffer, ClientStatus value) {

	EnterCriticalSection(&buffer->criticalSection);
	
	if (buffer->bufferSize >= MAX_BUFF_SIZE) {
		LeaveCriticalSection(&buffer->criticalSection);
		return false;
	}

	buffer->buffer[buffer->pushIdx] = value;
	buffer->pushIdx++;

	if (buffer->pushIdx == buffer->bufferSize) {
		buffer->pushIdx = 0;
	}

	buffer->count++;
	LeaveCriticalSection(&buffer->criticalSection);
	return true;
}

bool PopBuffer(CBuffer *buffer, ClientStatus *retVal) {

	EnterCriticalSection(&buffer->criticalSection);

	if (buffer->count == 0) {
		LeaveCriticalSection(&buffer->criticalSection);
		return false;
	}

	*retVal = buffer->buffer[buffer->popIdx];
	buffer->popIdx++;

	if (buffer->popIdx == buffer->bufferSize) {
		buffer->popIdx = 0;
	}

	buffer->count--;

	LeaveCriticalSection(&buffer->criticalSection);
	return true;
}


void PrintBuffer(CBuffer *buffer){

	EnterCriticalSection(&buffer->criticalSection);

	for (size_t i = 0; i < buffer->count; i++){
		PrintStruct(&buffer->buffer[i]);
		printf("\n");
	}

	LeaveCriticalSection(&buffer->criticalSection);
}