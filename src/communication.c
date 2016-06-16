#include "communication.h"

/* Initialize an empty message list */
MessageList initList() {
	MessageList mq;

	mq.first = NULL;
	mq.last = NULL;
	mq.size = 0;

	return mq;
}

/* Add a message at the end of the list */
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

/* Pop and return the first message of the list */
Message poll(MessageList* mq) {
	Message msg = mq->first->message;

	if (mq->size == 1) {
		mq->last = NULL;
	}
	mq->first = mq->first->next;
	mq->size--;

	return msg;
}

/* Remove every messages from the list, thus freeing previously allocated memory */
void removeList(MessageList* mq) {
	while (mq->size > 0) {
		poll(mq);
	}
}