#include "closest_serial.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


double combine_lr(struct Point P[], size_t n, struct Point mid_point, double d)
{
    struct Point *point = malloc(sizeof(struct Point) * n);
    int curr = 0;
    for(int i = 0; i < n; i++){
        if(abs(P[i].x - mid_point.x) < d){
            point[curr].x = P[i].x;
            point[curr].y = P[i].y;
            curr++;
        }
    }
    qsort(point, curr, sizeof(struct Point), compare_y);
    
    double curr_d;
    double mid_distance = d;

    for(int i = 0; i < curr; i++){
        for(int j = i + 1; j < curr; j++){
            if(abs(point[i].y - point[j].y) < d){
                curr_d = dist(point[i], point[j]);
                if (curr_d < mid_distance){
                    mid_distance = curr_d;
                }
            }
        }
    }
    free(point);
    return mid_distance;
}


double _closest_serial(struct Point P[], size_t n)
{
    struct Point p_mid = P[n/2];
    
    if(n <= 3){
        return brute_force(P, n);
    }

    double dl = _closest_serial(P, n/2);

    double dr = _closest_serial(&P[n/2], n - n/2);

    double distance;

    if(dl < dr){
        distance = dl;
    } else{
        distance = dr;
    }
    return combine_lr(P, n, p_mid, distance);
}


double closest_serial(struct Point P[], size_t n)
{
    qsort(P, n, sizeof(struct Point), compare_x);
    return _closest_serial(P, n);
}
