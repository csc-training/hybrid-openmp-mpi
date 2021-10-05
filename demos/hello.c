#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("Hello world!\n");

#pragma omp parallel
    {
        printf(".. thread here.\n");
    }

    return 0;
}
