#include "queue.h"

// create queue from list.c
Queue *queue_create(int size)
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->head = list_create(size);
    queue->tail = queue->head;
    queue->size = list_indexof(queue->head, size - 1);
    queue->count = size;
    return queue;
}