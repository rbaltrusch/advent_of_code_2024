#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "list.h"
#include "vector.h"

#define ASCII_SIZE 256
#define EMPTY '.'

void freePositions(List** lists)
{
    for(int i = 0; i < ASCII_SIZE; i++)
    {
        deleteList(lists[i]);
    }
}

void addToPositions(List** lists, const char type, int position)
{
    List* list = lists[type];
    addToList(list, position);
}

Vector readInput(List** lists, FILE* file)
{
    int character;
    bool widthLocked = false;
    int height = 1;
    int width = 0;
    int counter = 0;
    while ((character = getc(file)) != EOF)
    {
        if (character == '\n')
        {
            height++;
            widthLocked = true;
            continue;
        }
        if (!widthLocked) width++;

        if (character != EMPTY)
        {
            addToPositions(lists, character, counter);
        }
        counter++;
    }

    return (Vector) {width, height};
}

Vector calculatePosition(const Vector size, const int position)
{
    const int height = size.y;
    const int y = position / height;
    const int x = position - y * height;
    return (Vector) {x, y};
}

int calculateIndex(const Vector size, const Vector point)
{
    return point.x + point.y * size.y;
}

/* Only adds point as new antinode if not already in list of antinodes */
void addAntiNode(List* antiNodes, const Vector dimensions, const Vector point)
{
    if (!checkInBounds(dimensions, point)) return;

    const int index = calculateIndex(dimensions, point);
    if (!containsElement(antiNodes, index))
    {
        addToList(antiNodes, index);
    }
}

int main()
{
    List* lists[ASCII_SIZE];
    for (int i = 0; i < ASCII_SIZE; i++)
    {
        lists[i] = createNewList();
    }

    FILE* file;
    file = fopen("data.txt", "r");
    if (!file) return 1;
    Vector dims = readInput(lists, file);
    fclose(file);

    List* antiNodes = createNewList(); // HACK: should normally be a Set of antiNode points
    for (int i = 0; i < ASCII_SIZE; i++)
    {
        List* list = lists[i];
        const int size = list->size;
        if (!size) continue;
        //printList(list);
        for (int j = 0; j < size; j++)
        {
            Vector point1 = calculatePosition(dims, getElement(list, j)->value);
            for (int k = j + 1; k < size; k++)
            {
                Vector point2 = calculatePosition(dims, getElement(list, k)->value);
                Vector diff = calculateDistance(point1, point2);
                Vector newPoint1 = addVectors(point1, flipVector(diff));
                Vector newPoint2 = addVectors(point2, diff);
                /*printf("---");
                printVector(point1);
                printVector(point2);
                printVector(diff);
                printVector(newPoint1);
                printVector(newPoint2);*/
                addAntiNode(antiNodes, dims, newPoint1);
                addAntiNode(antiNodes, dims, newPoint2);
                printf("antiNodes: %i\n", antiNodes->size);
            }
        }
    }

    freePositions(lists);
    deleteList(antiNodes);
    printf("done.\n");
    return 0;
}
