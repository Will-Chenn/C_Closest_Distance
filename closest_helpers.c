#include "closest_helpers.h"
#include <math.h>

int verbose = 0;

int compare_x(const void* a, const void* b) 
{
    return ((struct Point*)a)->x - ((struct Point*)b)->x;
} 

int compare_y(const void* a, const void* b) 
{ 
    return ((struct Point*)a)->y - ((struct Point*)b)->y;
} 

double dist(struct Point p1, struct Point p2) 
{
    return pow(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2), 0.5);
} 
