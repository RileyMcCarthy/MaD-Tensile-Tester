#include "StaticQueue.h"

void queue_init(StaticQueue *queue, void *buf, int max_size, int item_size) {
    queue->buf = buf;
    queue->max_size = max_size;
    queue->item_size = item_size;
    queue->front = 0;
    queue->rear = 0;
    queue->_lock = _locknew();
    if (queue->_lock ==  -1)
        return;
}

bool queue_push(StaticQueue *queue, void *data) {
    if (data == NULL)
        return false;
    
    while (!_locktry(queue->_lock))
        ;
    
    if (queue_isfull(queue))
    {
        _lockrel(queue->_lock);
        return false;
    }
    
    memcpy(&(queue->buf[queue->rear*queue->item_size]), data, queue->item_size);
    queue->rear++;
    if (queue->rear == queue->max_size)
        queue->rear = 0;

    _lockrel(queue->_lock);
    return true;
}

bool queue_pop(StaticQueue *queue, void *data) {
    while (!_locktry(queue->_lock))
        ;
    if (queue_isempty(queue)) {
        _lockrel(queue->_lock);
        return false;
    }
    
    if (data != NULL)  
        memcpy(data, &(queue->buf[queue->item_size*queue->front]), queue->item_size);
    queue->front++;
    if (queue->front == queue->max_size)
        queue->front = 0;
    
    _lockrel(queue->_lock);
    return true;
}


bool queue_isempty(StaticQueue *queue) {
    return queue->rear == queue->front;
}

bool queue_isfull(StaticQueue *queue) {
    return queue->rear == queue->front -1 || (queue->front == 0 && queue->rear == queue->max_size - 1);
}

void queue_empty(StaticQueue *queue)
{
    while (!_locktry(queue->_lock))
        ;
    queue->front = 0;
    queue->rear = 0;
    _lockrel(queue->_lock);
}
