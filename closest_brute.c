#include "closest_brute.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


double brute_force(struct Point P[], size_t n) 
{ 
    if(n < 2){
        perror("invalid points provided here");
        exit(1);
    }

    double min_distance = dist(P[0], P[1]);
    double curr;
    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            curr = dist(P[i], P[j]);
            if(curr < min_distance){
                min_distance = curr;
            }
        }
    }

    return min_distance;
}
