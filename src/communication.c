#include "communication.h"

MessageList initList() {
	MessageList mq;

	mq.first = NULL;
	mq.last = NULL;
	mq.size = 0;

	return mq;
}

void offer(MessageList* mq, Message msg) {
	MessageListElement* e = (MessageListElement*)malloc(sizeof(MessageListElement));
	
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

Message poll(MessageList* mq) {
	Message msg = mq->first->message;

	if (mq->size == 1) {
		mq->last = NULL;
	}
	mq->first = mq->first->next;
	mq->size--;

	return msg;
}

void removeList(MessageList* mq) {
	while (mq->size > 0) {
		poll(mq);
	}
}