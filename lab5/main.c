#include <stdlib.h> // for exit
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#define MAX_INPUT 100000000
#define NUM_THREAD 4

int counter[NUM_THREAD] = {0, 0, 0, 0};
int n = 0;


void* genPoints(void* id) {
    int index = (int*) id;

    unsigned int seed = rand() % 10000;

    float x_axis;
    float y_axis;

    int i = 1;

    while(i <= n) {
        x_axis = (rand_r(&seed) - rand_r(&seed))*1.0 / (RAND_MAX *1.0);
        y_axis = (rand_r(&seed) - rand_r(&seed)) *1.0 / (RAND_MAX *1.0);

        if ( x * x + y * y <= 1)
            counter[index]++;

        i++;
    }
    pthread_exit(0);
}

int main(int argc, char **argv) {

    if (argc < 2 ) {
        printf("Format: %s <num>\n", argv[0]);
        exit(-1);
    }

    if (argv[1] == NULL || argv[1] <= 0) {
        printf("Enter positive value\n" );
        return 0;
    }

    int numberOfPoints = atoi(argv[1]);
    printf("Number of Points : %d\n", numberOfPoints);

    n = numberOfPoints/ NUM_THREAD; // 100 / 4 => each thread handle 25points

    pthread_t tid[NUM_THREAD];

    srand(time(0));
    clock_t beginTime, endTime;
    double elapsedTime;

    beginTime = clock();
    int i;
    for (i  = 0; i < NUM_THREAD; i++) {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tid[i], &attr, genPoints, &i);
    }

    //Wait until thread is done its work
    for (i  = 0; i < NUM_THREAD; i++) {
        pthread_join(tid[i], NULL);
        count += counter[i];

    }

    float valueOfPi;
    valueOfPi = 4 * (1.0 * numberOfPointsInCircle) / (1.0 * numberOfPoints);
    printf("pi : %.5f\n", valueOfPi);

    endTime = clock();


    elapsedTime = (double) (endTime - beginTime);
    printf("Elapsed time%f\n", elapsedTime / CLOCKS_PER_SEC);

    return 0;
}
