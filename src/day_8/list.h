#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Node
{
    int value;
    struct Node* next;
} Node;

typedef struct
{
    Node* head;
    Node* tail;
    unsigned int size;
} List;

List* createNewList()
{
    List* list = (List*) malloc(sizeof(List));
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void deleteList(List* list)
{
    Node* current = list->head;
    while(current != NULL)
    {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

void addToList(List* list, const int value)
{
    Node* current = (Node*) malloc(sizeof(Node));
    current->next = NULL;
    current->value = value;

    const int size = list->size;
    if (!size)
    {
        list->head = current;
        list->tail = current;
    }
    else
    {
        list->tail->next = current;
        list->tail = current;
    }
    list->size = size + 1;
}

Node* getElement(const List* list, const int index)
{
    if (index >= list->size) return NULL;

    int counter = 0;
    Node* current = list->head;
    while(current != NULL)
    {
        if (index == counter) return current;
        current = current->next;
        counter++;
    }
    return NULL;
}

bool containsElement(const List* list, const int value)
{
    Node* current = list->head;
    while(current != NULL)
    {
        if (current->value == value) return true;
        current = current->next;
    }
    return false;
}

void printList(const List* list)
{
    Node* current = list->head;
    printf("{");
    while(current != NULL)
    {
        Node* next = current->next;
        const char* delim = next == NULL ? "" : ",";
        printf("%i%s", current->value, delim);
        current = next;
    }
    printf("}\n");
}
