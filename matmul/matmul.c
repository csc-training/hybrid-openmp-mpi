//===-- matmul.c - Different implementations of matrix multiplies -*- C -*-===//
//
// Part of the LOMP Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

#define N 3072

#define DUMP_MATRIX 0
#define DUMP_TASKS  0


void matmul(float * C, float * A, float * B, size_t n) {
    const size_t bf = 256;
    if (n % bf != 0) {
        printf("Blocking factor does not divide matrix size!\n");
        exit(EXIT_FAILURE);
    }
    {
        // work on the blocks of the matrix
        for (size_t ib = 0; ib < n; ib += bf)
            for (size_t kb = 0; kb < n; kb += bf)
                for (size_t jb = 0; jb < n; jb += bf) {
                    {
#if DUMP_TASKS
                        printf("task: C[%ld,%ld] += A[%ld,%ld] o B[%ld,%ld]\n", ib, jb, ib, kb, kb, jb);
#endif                        
                        // work on a single block``
                        for (size_t i = ib; i < (ib + bf); ++i)
                            for (size_t k = kb; k < (kb + bf); ++k)
                                for (size_t j = jb; j < (jb + bf); ++j)
                                    C[i * n + j] += A[i * n + k] * B[k * n + j];
                    }
                }
    }
}

void init_mat(float * C, float * A, float * B, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            C[i * n + j] = 0.0;
            A[i * n + j] = 0.5;
            B[i * n + j] = 0.25;
        }
    }
}

void dump_mat(float * mtx, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            printf("%f ", mtx[i * n + j]);
        }
        printf("\n");
    }
}

float sum_mat(float * mtx, size_t n) {
    float sum = 0.0;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            sum += mtx[i * n + j];
        }
    }
    return sum;
}

int main(int argc, char * argv[]) {
    double ts, te;
    double t_seq;

    float * C;
    float * A;
    float * B;

    // If number of arguments is not 1, print help
    if (argc != 2) {
        printf("%s: matrix_size\n", argv[0]);
        return EXIT_FAILURE;
    } 
    const int  n = atoi(argv[1]); // matrix size

    C = (float *) malloc(sizeof(*C) * n * n);
    A = (float *) malloc(sizeof(*A) * n * n);
    B = (float *) malloc(sizeof(*B) * n * n);

    init_mat(C, A, B, n);
    ts = omp_get_wtime();
    matmul(C, A, B, n);
    te = omp_get_wtime();
#if DUMP_MATRIX
    dump_mat(C, n);
#endif
    printf("Sum of matrix:    %f, wall time %lf\n",
           sum_mat(C, n), (te-ts));

    return EXIT_SUCCESS;
}
