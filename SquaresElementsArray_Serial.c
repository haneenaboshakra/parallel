#include <stdio.h>
#include <omp.h>
#include <stdlib.h> // For rand() and srand()
#include <time.h>   // For time()

#define ARRAY_SIZE 100

int main(int argc, char* argv[])
{
    int a[ARRAY_SIZE]; // shared

    // Seed the random number generator
    srand(time(NULL));
    int i;

    // Fill the array with random numbers
    #pragma omp parallel num_threads(4)
    {
        #pragma omp for
        for (i = 0; i < ARRAY_SIZE; i++)
        {
            a[i] = rand() % 100; // Generate a random number between 0 and 99
            printf("Thread Setting a[%d] = %d\n",i, a[i]); // print the random number
        }
        #pragma omp for
        for (i = 0; i < ARRAY_SIZE; i++)
        {
            a[i] = a[i] * a[i]; // Square the element and override the array

        }
        // Printing the array values
        #pragma omp single
        printf("Final array:\n");
        #pragma omp for ordered
        for (int i = 0; i < ARRAY_SIZE; i++)
        #pragma omp ordered
        {
            printf("a[%d] = %2d\n", i, a[i]);
        }
    }
    return 0;
}
