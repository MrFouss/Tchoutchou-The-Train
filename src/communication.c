#include "communication.h"

MessageQueue initQueue() {
	MessageQueue mq;

	mq.first = NULL;
	mq.last = NULL;
	mq.size = 0;

	return mq;
}

void offer(MessageQueue* mq, Message msg) {
	MessageQueueElement* e = (MessageQueueElement*)malloc(sizeof(MessageQueueElement));
	
	e->message = msg;
	e->next = NULL;
	if (mq->size == 0) {
		mq->first = e;
	} else {
		mq->last->next = e;
	}
	mq->last = e;
	mq->size++;
}

Message poll(MessageQueue* mq) {
	Message msg = mq->first->message;

	if (mq->size == 1) {
		mq->last = NULL;
	}
	mq->first = mq->first->next;
	mq->size--;

	return msg;
}

void removeQueue(MessageQueue* mq) {
	while (mq->size > 0) {
		poll(mq);
	}
}