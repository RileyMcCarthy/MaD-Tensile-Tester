#include <stdbool.h>
#include <stdlib.h>

typedef struct StaticQueue_t {
    void *buf;
    int front;
    int rear;
    int size;
    int max_size;
    int item_size;
    int _lock;
} StaticQueue;

bool queue_init(StaticQueue *queue, void *buf, int max_size, int item_size);
bool queue_push(StaticQueue *queue, void *data);
bool queue_pop(StaticQueue *queue, void *data);
void queue_empty(StaticQueue *queue);
bool queue_isempty(StaticQueue *queue);
