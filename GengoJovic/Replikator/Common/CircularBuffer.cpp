#include "CircularBuffer.h"


//#include "stdafx.h"

void InitializeBuffer(CBuffer **buffer, int bufferSize) {
	*buffer = (CBuffer*)malloc(sizeof(CBuffer));
	InitializeCriticalSection(&(*buffer)->criticalSection);
	(*buffer)->bufferSize = bufferSize;
	(*buffer)->buffer = (int*)malloc(sizeof(int)*bufferSize);
	(*buffer)->count = 0;
	(*buffer)->popIdx = 0;
	(*buffer)->pushIdx= 0;

}

void DestroyBuffer(CBuffer **buffer) {
	free((*buffer)->buffer);
	DeleteCriticalSection(&(*buffer)->criticalSection);
	free(*buffer);
}

void ExpandBuffer(CBuffer *buffer) {
	int newBufferSize = buffer->bufferSize * 2;
	int *newBuffer = (int *)calloc(newBufferSize, sizeof(int));

	int dev = buffer->bufferSize - buffer->pushIdx;	// razlika izmedju push i bufferSize

	memcpy(newBuffer, buffer->buffer + buffer->pushIdx, dev * sizeof(int));
	memcpy(newBuffer + dev, buffer->buffer, (buffer->bufferSize - dev) * sizeof(int));

	free(buffer->buffer);

	buffer->popIdx = 0;
	buffer->pushIdx = buffer->count;
	buffer->buffer = newBuffer;
	buffer->bufferSize = newBufferSize;

}

void NarrowBuffer(CBuffer *buffer) {
	int newBufferSize = buffer->bufferSize / 2;
	int *newBuffer = (int*)calloc(newBufferSize, sizeof(int));

	if (buffer->popIdx <= buffer->pushIdx) {
		int dev = buffer->pushIdx - buffer->popIdx;
		memcpy(newBuffer, buffer->buffer + buffer->popIdx, dev * sizeof(int));
	}
	else {
		int dev = buffer->bufferSize - buffer->popIdx;
		memcpy(newBuffer, buffer->buffer + buffer->popIdx, dev * sizeof(int));
		memcpy(newBuffer + dev, buffer->buffer, buffer->pushIdx * sizeof(int));
	}

	buffer->popIdx = 0;
	buffer->pushIdx = buffer->count;
	buffer->buffer = newBuffer;
	buffer->bufferSize = newBufferSize;
	
}


bool PushBuffer(CBuffer *buffer, int number) {
	EnterCriticalSection(&buffer->criticalSection);
	if(buffer->count == buffer->bufferSize){
		if (buffer->bufferSize >= MAX_BUFF_SIZE) {
			LeaveCriticalSection(&buffer->criticalSection);
			return false;
		}
		printf("\nExpanding buffer");
		ExpandBuffer(buffer);
	}

	buffer->buffer[buffer->pushIdx] = number;
	buffer->pushIdx++;

	if (buffer->pushIdx == buffer->bufferSize) {
		buffer->pushIdx = 0;
	}

	buffer->count++;
	LeaveCriticalSection(&buffer->criticalSection);
	return true;
}

bool PopBuffer(CBuffer *buffer, int *retVal) {
	EnterCriticalSection(&buffer->criticalSection);
	if (buffer->count == 0) {
		LeaveCriticalSection(&buffer->criticalSection);
		return false;
	}

	int usage = (int)((float)buffer->count / (float)buffer->bufferSize) * 100;
	if (usage < 25 && buffer->bufferSize > MIN_BUFF_SIZE) {
		printf("\nBuffer is %d%% full...", usage);
		printf("\nBuffer is narrowing...");

		NarrowBuffer(buffer);
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
