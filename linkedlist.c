#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

struct linkedlist
{
    struct linkedlist_node *first;
    // TODO: define linked list metadata
    int listSize;
};

struct linkedlist_node
{
    // TODO: define the linked list node
    int key;
    int value;
    struct linkedlist_node *next;
};

typedef struct linkedlist_node linkedlist_node_t; // just for shorter code

linkedlist_t *ll_init()
{
    // TODO: create a new linked list

    // We have done this TODO for you as an example of how to use malloc().
    // You might want to read more about malloc() from Linux Manual page.
    // Usually free() should be used together with malloc(). For example,
    // the linkedlist you are currently implementing usually have free() in the
    // ll_delete() function. Since we are not asking you to implement
    // the ll_delete() function, you will not be using free() in this case.

    // First, you use C's sizeof function to determine
    // the size of the linkedlist_t data type in bytes.
    // Then, you use malloc to set aside that amount of space in memory.
    // malloc returns a void pointer to the memory you just allocated, but
    // we can assign this pointer to the specific type we know we created
    linkedlist_t *list = malloc(sizeof(linkedlist_t));

    // TODO: set metadata for your new list and return the new list
    list->first = NULL;
    list->listSize = 0;
    return list;
}

void ll_add(linkedlist_t *list, int key, int value)
{
    // TODO: create a new node and add to the front of the linked list if a
    // node with the key does not already exist.
    // Otherwise, replace the existing value with the new value.
    if (list == NULL)
        return; // for sure that list != null

    linkedlist_node_t *pointer = list->first;
    while (pointer != NULL)
    {
        if (pointer->key == key)
        {
            // replace if key already exist
            pointer->value = value;
            return;
        }
        pointer = pointer->next;
    }

    // key does not already exist -> add new node to the beginning of linkedlist
    linkedlist_node_t *newNode = malloc(sizeof(linkedlist_node_t));
    newNode->key = key;
    newNode->value = value;
    newNode->next = list->first;
    list->first = newNode;
    list->listSize++;
}

int ll_get(linkedlist_t *list, int key)
{
    // TODO: go through each node in the linked list and return the value of
    // the node with a matching key.
    // If it does not exist, return 0.
    if (list == NULL)
        return 0; // for sure that list != null
    linkedlist_node_t *pointer = list->first;
    while (pointer != NULL)
    {
        if (pointer->key == key)
            return pointer->value;
        pointer = pointer->next;
    }
    return 0;
}

int ll_size(linkedlist_t *list)
{
    // TODO: return the number of nodes in this linked list
    if (list == NULL)
        return 0; // for sure that list != null
    return list->listSize;
}
