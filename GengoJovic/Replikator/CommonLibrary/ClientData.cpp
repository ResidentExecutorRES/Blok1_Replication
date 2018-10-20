#include "ClientData.h"


char* Serialize(char c) {
	char* retVal = (char*)malloc(1 + sizeof(time_t));
	retVal[0] = c;

	time_t now = time(0);
	strftime(retVal + 1, 100, "%Y-%m-%d %H:%M:%S.000", localtime(&now));
	//memcpy(retVal + 1, &c.timeStamp, sizeof(time_t));

	return retVal;
}
ClientStatus Deserialize(char *data) {
	ClientStatus retVal;
	retVal.statusC = (StatusC)data[0];
	retVal.timeStamp = StringToTime((data + 1));


	return retVal;
}

tm StringToTime(char *data) {
	//time_t retVal;
	struct tm structTm;
	char year[4], month[2];/*, day[2], hour[2], min[2], sec[2];*/


	structTm.tm_year = atoi(strncpy(year, data, 4));
	structTm.tm_mon = atoi(strncpy(month, data + 5, 2));
	structTm.tm_mday = atoi(strncpy(month, data + 8, 2));
	structTm.tm_hour = atoi(strncpy(month, data + 11, 2));
	structTm.tm_min = atoi(strncpy(month, data + 14, 2));
	structTm.tm_sec = atoi(strncpy(month, data + 17, 2));

	return structTm;
}

//void PrintStruct(ClientStatus c) {
//
//	if (c.statusC == 49) {
//		printf("Client status: OPEN. TimeStamp: ");
//	}
//	else {
//		printf("Client status: CLOSE. TimeStamp: ");
//	}
//
//	printf("%d-%d-%d %d:%d:%d.000\n", c.timeStamp.tm_year,
//		c.timeStamp.tm_mon,
//		c.timeStamp.tm_mday,
//		c.timeStamp.tm_hour,
//		c.timeStamp.tm_min,
//		c.timeStamp.tm_sec);
//}

void PrintStruct(ClientStatus *c) {
	
		if (c->statusC == 49) {
			printf("Client status: OPEN. TimeStamp: ");
		}
		else {
			printf("Client status: CLOSE. TimeStamp: ");
		}
	
		printf("%d-%d-%d %d:%d:%d.000\n\n", c->timeStamp.tm_year,
			c->timeStamp.tm_mon,
			c->timeStamp.tm_mday,
			c->timeStamp.tm_hour,
			c->timeStamp.tm_min,
			c->timeStamp.tm_sec);
}