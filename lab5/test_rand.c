#include <stdlib.h> // for exit
#include <stdio.h>
#include <time.h>

#define MAX_INPUT 100000000

int main(int argc, char **argv) {

    srand(time(0));
    clock_t beginTime, endTime;
    double elapsedTime;


    float x_axis;
    float y_axis;

    int i = 1;
    unsigned int seed = rand() % 10000;
    beginTime = clock();

    while(i <= MAX_INPUT) {
        x_axis = (rand_r(&seed) - rand_r(&seed) ) * 1.0 / (RAND_MAX *1.0);
        y_axis = (rand_r(&seed) - rand_r(&seed) ) * 1.0 / (RAND_MAX * 1.0);
        printf("%.5f %.5f\n", x_axis, y_axis);
        i++;
    }

    endTime = clock();
    elapsedTime = (double) (endTime - beginTime);
    printf("%f\n", elapsedTime / CLOCKS_PER_SEC);

    return 0;
}
