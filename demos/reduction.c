#include <stdio.h>

int main(int argc, char *argv[])
{
    int total = 0;
    int new, sum;
#pragma omp parallel shared(total) private(sum,new)
    {
#pragma omp single
        total = 800;
        int i;
        sum = 0;
        for (i=0; i < 4; i++) {
            new = i;
            sum += new;
        }
        printf("new=%d  sum=%d\n", new, sum);
#pragma omp critical(addup)
        total += sum;
#pragma omp barrier
#pragma omp master
        printf("Grand total is: %d\n", total);
    }
    return 0;
}
