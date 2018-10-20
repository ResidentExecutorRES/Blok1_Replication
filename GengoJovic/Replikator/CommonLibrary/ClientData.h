#pragma once
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define _CRT_SECURE_NO_WARNINGS
#define _XOPEN_SOURCE 700
//#pragma warning(suppress : 4996)
#pragma warning(disable : 4996)

typedef enum StatusC { Close = 0, Open }StatusC;
typedef struct ClientStatus {
	StatusC statusC;
	tm timeStamp;
}ClientStatus;

char* Serialize(char c);
ClientStatus Deserialize(char *data);
void PrintStruct(ClientStatus *c);
tm StringToTime(char *data);
