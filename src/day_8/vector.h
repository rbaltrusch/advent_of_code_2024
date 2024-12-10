#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Vector
{
    int x;
    int y;
} Vector;

Vector flipVector(struct Vector vector)
{
    return (Vector) {-vector.x, -vector.y};
}

Vector calculateDistance(const Vector point1, const Vector point2)
{
    const int x = point2.x - point1.x;
    const int y = point2.y - point1.y;
    return (Vector) {x, y};
}

Vector addVectors(const Vector point, const Vector distance)
{
    const int x = point.x + distance.x;
    const int y = point.y + distance.y;
    return (Vector) {x, y};
}

void printVector(const Vector point)
{
    printf("Vector(x=%i, y=%i)\n", point.x, point.y);
}

bool checkInBounds(const Vector bounds, const Vector point)
{
    return 0 <= point.x && point.x < bounds.x && 0 <= point.y && point.y < bounds.y;
}
