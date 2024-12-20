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
void addAntiNodes(List* antiNodes, const Vector dimensions, const Vector point, const Vector diff, const int maxAntiNodes)
{
    int counter = 0;
    Vector newPoint = point;
    while(counter < maxAntiNodes && checkInBounds(dimensions, newPoint))
    {
        const int index = calculateIndex(dimensions, newPoint);
        if (!containsElement(antiNodes, index))
        {
            // since this is a List and not a Set, this workaround List usage
            // results in O(n^2) time complexity
            addToList(antiNodes, index);
        }
        newPoint = addVectors(newPoint, diff);
        counter++;
    }
}

List* determineAntiNodes(List** lists, Vector dims, const int maxAntiNodes)
{
    List* antiNodes = createNewList(); // HACK: should normally be a Set of antiNode points
    for (int i = 0; i < ASCII_SIZE; i++)
    {
        List* list = lists[i];
        const int size = list->size;
        if (!size) continue;
        for (int j = 0; j < size; j++)
        {
            Vector point1 = calculatePosition(dims, getElement(list, j)->value);
            for (int k = j + 1; k < size; k++)
            {
                Vector point2 = calculatePosition(dims, getElement(list, k)->value);
                Vector diff = calculateDistance(point1, point2);
                addAntiNodes(antiNodes, dims, point1, flipVector(diff), maxAntiNodes);
                addAntiNodes(antiNodes, dims, point2, diff, maxAntiNodes);
            }
        }
    }
    return antiNodes;
}

void printAntiNodeMap(List* antiNodes, Vector dims)
{
    for (int y = 0; y < dims.y; y++)
    {
        for (int x = 0; x < dims.x; x++)
        {
            char c = containsElement(antiNodes, calculateIndex(dims, (Vector){x, y})) ? 'X' : '.';
            printf("%c", c);
        }
        printf("\n");
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

    // part 1: only one anti-node in each direction
    List* antiNodes = determineAntiNodes(lists, dims, 1);
    printf("antiNodes: %i\n", antiNodes->size);
    deleteList(antiNodes);

    // part 2: unlimited anti-nodes in each direction
    antiNodes = determineAntiNodes(lists, dims, dims.x + dims.y);
    printf("antiNodes: %i\n", antiNodes->size);
    printAntiNodeMap(antiNodes, dims);

    deleteList(antiNodes);
    freePositions(lists);
    printf("done.\n");
    return 0;
}
