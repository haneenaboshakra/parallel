#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NRA 10     /* number of rows in matrix A */
#define NCA 10     /* number of columns in matrix A */
#define NCB 10     /* number of columns in matrix B */

int main() {
    int i, j, k;
    int a[NRA][NCA], b[NCA][NCB], c[NRA][NCB], sum;

    /* Set the number of threads for parallel regions */
    omp_set_num_threads(4);

    /* Initialize random seed */
    srand(1);
    #pragma omp parallel
    {
        #pragma omp for private(j)
        /* Initialize matrix a with random values */
        for (i = 0; i < NRA; i++) {
            for (j = 0; j < NCA; j++) {
                int tid = omp_get_thread_num();
                a[i][j] = rand() % 10; // Random value between 0 and 9
                printf("Thread %d filling the matrix a[%d][%d] = %d A\n", tid, i, j, a[i][j]);

            }
        }
        /* Initialize matrix b with random values */
        #pragma omp single
        printf("\n-------------------\n\n");
        #pragma omp for private(j)
        for (i = 0; i < NCA; i++) {
            for (j = 0; j < NCB; j++) {
                int tid = omp_get_thread_num();
                b[i][j] = rand() % 10; // Random value between 0 and 9
                printf("Thread %d filling the matrix a[%d][%d] = %d B\n", tid, i, j, b[i][j]);
            }
        }

        /* Perform matrix multiplication */
        #pragma omp for ordered private(j,k) reduction (+ : sum)
        for (i = 0; i < NRA; i++) {
            int tid = omp_get_thread_num();
            #pragma omp ordered
            for (j = 0; j < NCB; j++) {
                sum = 0;
                for (k = 0; k < NCA; k++) {
                    sum += a[i][k] * b[k][j];
                }
                c[i][j] = sum;
            }
        }



    /* Display the matrices and the result */
        #pragma omp single 
        printf("\nMatrix A:\n");
        #pragma omp for ordered private(j)
        for (i = 0; i < NRA; i++) {
            #pragma omp ordered
            for (j = 0; j < NCA; j++) {
                // #pragma omp critical
                printf("%d ", a[i][j]);
            }
            printf(" | \n");
        }

        #pragma omp single 
        printf("\nMatrix B:\n");
        #pragma omp for ordered private(j)
        for (i = 0; i < NCA; i++) {
            #pragma omp ordered
            for (j = 0; j < NCB; j++) {
                printf("%d ", b[i][j]);
            }
            printf(" | \n");
        }
        #pragma omp single
        printf("\nMatrix C = A x B:\n");
        #pragma omp for ordered private(j)
        for (i = 0; i < NRA; i++) {
            #pragma omp ordered
            for (j = 0; j < NCB; j++) {
                printf("%d ", c[i][j]);
            }
            printf("\n");
        }
        
    }






    // printf("\nMatrix A:\n");
    //     for (i = 0; i < NRA; i++) {
    //         for (j = 0; j < NCA; j++) {
    //             printf("%d ", a[i][j]);
    //         }
    //         printf("\n");
    //     }
    //  printf("\nMatrix C = A x B:\n");
    // for (i = 0; i < NRA; i++) {
    //     for (j = 0; j < NCB; j++) {
    //         printf("%d ", c[i][j]);
    //     }
    //     printf("\n");
    // }
    return 0;
}
