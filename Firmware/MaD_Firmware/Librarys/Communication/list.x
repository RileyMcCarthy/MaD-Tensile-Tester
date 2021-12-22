#include "list.h"

// create linked list of Nodes
Node *list_create(int size)
{
    Node *head = NULL;
    Node *current = NULL;
    Node *newNode = NULL;
    for (int i = 0; i < size; i++)
    {
        newNode = (Node *)malloc(sizeof(Node));
        newNode->data = NULL;
        newNode->next = NULL;
        if (head == NULL)
        {
            head = newNode;
            current = newNode;
        }
        else
        {
            current->next = newNode;
            current = newNode;
        }
    }
    return head;
}

void list_print(Node *head)
{
    Node *current = head;
    while (current != NULL)
    {
        printf("%d\n", current->data);
        current = current->next;
    }
}

Node *list_indexof(Node *head, int index)
{
    Node *current = head;
    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }
    return current;

    void list_insert(Node * *head, void *data, int index)
    {
        Node *current = *head;
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->data = data;
        newNode->next = NULL;
        if (index == 0)
        {
            newNode->next = *head;
            *head = newNode;
        }
        else
        {
            int i = 0;
            while (current != NULL && i < index - 1)
            {
                current = current->next;
                i++;
            }
            if (current != NULL)
            {
                newNode->next = current->next;
                current->next = newNode;
            }
            else
            {
                printf("Index out of bounds\n");
            }
        }
    }

    void list_remove(Node * *head, int index)
    {
        Node *current = *head;
        if (index == 0)
        {
            *head = current->next;
            free(current);
        }
        else
        {
            int i = 0;
            while (current != NULL && i < index - 1)
            {
                current = current->next;
                i++;
            }
            if (current != NULL)
            {
                Node *next = current->next->next;
                free(current->next);
                current->next = next;
            }
            else
            {
                printf("Index out of bounds\n");
            }
        }
    }