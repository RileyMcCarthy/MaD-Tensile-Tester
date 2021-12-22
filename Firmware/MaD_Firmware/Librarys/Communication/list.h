#include <stdlib.h>

typedef struct node_t
{
    void *data;
    struct node_t *next;
} Node;

Node *list_create(int size);
void list_print(Node *head);
void list_insert(Node **head, void *data, int index);
void list_remove(Node **head, int index);
Node *list_indexof(Node *head, int index);