#include "CircularBuffer.h"
#include <stdio.h>

void InitializeBuffer(CBuffer **buffer, int bufferSize) {
	*buffer = (CBuffer*)malloc(sizeof(CBuffer));
	(*buffer)->bufferSize = bufferSize;
	(*buffer)->buffer = (ClientStatus*)malloc(sizeof(ClientStatus)*bufferSize);
	(*buffer)->count = 0;
	(*buffer)->popIdx = 0;
	(*buffer)->pushIdx = 0;

}

void DestroyBuffer(CBuffer **buffer) {
	free((*buffer)->buffer);
	free(*buffer);
}

bool PushBuffer(CBuffer *buffer, ClientStatus value) {
	buffer->buffer[buffer->pushIdx] = value;
	buffer->pushIdx++;

	if (buffer->pushIdx == buffer->bufferSize) {
		buffer->pushIdx = 0;
	}

	buffer->count++;
	return true;
}

bool PopBuffer(CBuffer *buffer, ClientStatus *retVal) {
	*retVal = buffer->buffer[buffer->popIdx];
	buffer->popIdx++;

	if (buffer->popIdx == buffer->bufferSize) {
		buffer->popIdx = 0;
	}

	buffer->count--;
	return true;
}


void PrintBuffer(CBuffer *buffer){
	for (size_t i = 0; i < buffer->count; i++){
		PrintStruct(&buffer->buffer[i]);
		printf("\n");
	}
}