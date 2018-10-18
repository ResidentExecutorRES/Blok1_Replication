#pragma once
//#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include"ClientData.h"

#define MAX_BUFF_SIZE 10240
#define MIN_BUFF_SIZE 10


typedef struct CBuffer {
	ClientStatus buffer;
	struct CBuffer *link;
} CBuffer;

void Add(struct CBuffer **front, struct CBuffer **rear, ClientStatus value);
void Remove(struct CBuffer **front, struct CBuffer **rear, ClientStatus *value);






