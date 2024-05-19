#include <omp.h>
#include <stdio.h>

static long num_steps = 100;
double step;


int main() {
    double time0 = omp_get_wtime();
    double pi = 0; // shared
    step = 1.0 / (double)num_steps;
    int i; // shared
    double x; //shared
    double sum; //shared
    sum = 0.0;

    #pragma omp parallel reduction (+ : sum)
    {
        #pragma omp for private(x)
            for (i = 0; i < num_steps; i = i + 1) {
                // printf("Hello from %d, i = %d\n", omp_get_thread_num(), i);
                x = (i + 0.5) * step;
                sum += 4.0 / (1.0 + x * x);
        }
        
    }
    
    pi += sum * step;
    printf("Value of PI: %f\n", pi);
    double time1 = omp_get_wtime(); 
    printf("Duration is: %f\n", time1 - time0);   
    return 0;
}
