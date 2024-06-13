#ifndef _QUEUE_H
#define _QUEUE_H

#include <malloc.h>
#include <stdint.h>
#include <stdio.h>

typedef struct queue queue;

queue *QueueInit(void);
void QueueFree(queue **);

int8_t QueuePushToHead(int8_t, queue *);
int8_t QueuePushToEnd(int8_t, queue *);

int8_t QueuePullFromHead(int8_t *, queue *);
int8_t QueuePullFromEnd(int8_t *, queue *);

int8_t QueueIncreaseCmp(int8_t, int8_t);
int8_t QueueDecreaseCmp(int8_t, int8_t);
int8_t QueueSort(queue *, int8_t(cmp)(int8_t, int8_t));

#endif // !_QUEUE_H
