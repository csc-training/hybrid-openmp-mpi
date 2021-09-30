#include <stdio.h>
#include <omp.h>

#define NX 102400

int main(void)
{
    long vecA[NX], vecB[NX];
    long sum;

    /* Initialization of the vectors */
    for (int i = 0; i < NX; i++) {
        vecA[i] = (long) i + 1;
        vecB[i] = -vecA[i];
    }


    sum = 0.0;

    // First sum 
    for (int i = 0; i < NX; i++) {
        sum += vecA[i];
    }
    // Second sum 
    for (int i = 0; i < NX; i++) {
        sum += vecB[i];
      }
    printf("Reduction sum:     %ld\n", sum);

    return 0;
}
