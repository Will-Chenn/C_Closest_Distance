#include "closest_parallel.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int pdepth = 0;

double _closest_parallel(struct Point P[], size_t n, int pdmax, int *pcount)
{
    static int num_forks = 0;

    qsort(P, n, sizeof(struct Point), compare_x);

    if(pdepth == pdmax || n <= 3){
        return _closest_serial(P, n);
    }

    int fd1[2];

    if(pipe(fd1) == -1){
        fprintf(stderr, "pipe 1st child failed\n");
        exit(1);
    }

    int pid;

    if((pid = fork()) < 0){
        fprintf(stderr, "fork 1st child failed\n");
        exit(1);
    }
    num_forks ++;

    if(pid == 0){    
        pdepth ++;
        close(fd1[0]);  

        struct Point *P1 = malloc(sizeof(struct Point) * n/2);

        for(int i = 0; i < n/2; i++){
            P1[i].x = P[i].x;
            P1[i].y = P[i].y;
        }

        double dl = _closest_parallel(P, n/2, pdmax, pcount);
        write(fd1[1], &dl, sizeof(double));
        close(fd1[1]);
        free(P1);
        exit(0);
    }

    int fd2[2];
    if(pipe(fd2) == -1){
        fprintf(stderr, "pipe 2nd child failed\n");
        exit(1);
    }

    if((pid = fork()) < 0){
        fprintf(stderr, "fork 2nd child failed\n");
        exit(1);
    }
    num_forks ++;   

    if(pid == 0){ 
        pdepth ++;
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);

        struct Point *P2 = malloc(sizeof(struct Point) * (n - n/2));

        for(int i = 0; i < (n - n/2); i++){
            P2[i].x = P[n/2 + i].x;
            P2[i].y = P[n/2 + i].y;
        }

        double dr = _closest_parallel(P2, n - n/2, pdmax, pcount);
        write(fd2[1], &dr, sizeof(double));
        close(fd2[1]);

        free(P2);
        exit(0);
    }

    double dl;
    double dr;
    close(fd1[1]);
    close(fd2[1]);

    read(fd1[0], &dl, sizeof(double));
    read(fd2[0], &dr, sizeof(double));

    close(fd1[0]);
    close(fd2[0]);

    double disance;

    if(dl < dr){
        disance = dl;
    } else{
        disance = dr;
    }

    return combine_lr(P, n, P[n/2], disance);
}


double closest_parallel(struct Point P[], size_t n, int pdmax, int *pcount)
{
    qsort(P, n, sizeof(struct Point), compare_x);

    return _closest_parallel(P, n, pdmax, pcount);
}
