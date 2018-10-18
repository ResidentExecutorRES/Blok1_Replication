#include "CircularBuffer.h"

void Add(CBuffer **front, CBuffer **rear, ClientStatus value) {
	CBuffer *temp;
	temp = (CBuffer*)malloc(sizeof(CBuffer));
	
	temp->buffer = value;
	temp->link = NULL;

	if (*rear == NULL) {
		*rear = temp;
		*front = *rear;
	}
	else {
		(*rear)->link = temp;
		*rear = temp;
	}
}
void Remove(CBuffer **front,CBuffer **rear, ClientStatus *value) {
	CBuffer *temp;
	if ((*front == *rear) && (*rear == NULL))
		return;

	*value = (*front)->buffer;
	temp = *front;
	*front = (*front)->link;

	if (*rear == temp)
		*rear = (*rear)->link;

	free(temp);
}
