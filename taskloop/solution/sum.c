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

    #pragma omp parallel
    #pragma omp single
    #pragma omp taskgroup task_reduction(+:sum)
    {
    #pragma omp task in_reduction(+:sum)
    {
    // First sum 
      #pragma omp taskloop in_reduction(+:sum)
      for (int i = 0; i < NX; i++) {
          sum += vecA[i];
      }
    }
    #pragma omp task in_reduction(+:sum)
    {
    // Second sum 
      #pragma omp taskloop in_reduction(+:sum)
      for (int i = 0; i < NX; i++) {
          sum += vecB[i];
      }
    }
    }
    printf("Reduction sum:                                   %ld\n",
           sum);

    return 0;
}
