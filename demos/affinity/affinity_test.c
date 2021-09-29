#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

/* Print outs to which core MPI rank and OpenMP thread is bind to.
   Performs also dummy calculation, increase in execution time indicates
   that cores are oversubscribed. Comparing times with single MPI task,
   single thread vs. full node gives also hints about effects of CPU
   frequency scaling.

   Note: compiler might optimize the calculation away, compile without
   optimization */

/*  mpicc -fopenmp -lmpi -lm -o out affinity_test.c */

/* Borrowed from util-linux-2.13-pre7/schedutils/taskset.c */
static char *cpuset_to_cstr(cpu_set_t *mask, char *str)
{
  char *ptr = str;
  int i, j, entry_made = 0;
  for (i = 0; i < CPU_SETSIZE; i++) {
    if (CPU_ISSET(i, mask)) {
      int run = 0;
      entry_made = 1;
      for (j = i + 1; j < CPU_SETSIZE; j++) {
	if (CPU_ISSET(j, mask)) run++;
	else break;
      }
      if (!run)
	sprintf(ptr, "%d,", i);
      else if (run == 1) {
	sprintf(ptr, "%d,%d,", i, i + 1);
	i++;
      } else {
	sprintf(ptr, "%d-%d,", i, i + run);
	i += run;
      }
      while (*ptr != 0) ptr++;
    }
  }
  ptr -= entry_made;
  *ptr = 0;
  return(str);
}


int main(int argc, char *argv[])
{
  int rank, thread_id, provided;
  cpu_set_t coremask;
  int niter = 40000000;  // number of iterations per FOR loop
  double x,y, z;
  
  char clbuf[7 * CPU_SETSIZE], hnbuf[64]; // core and hostname strings

  MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  memset(clbuf, 0, sizeof(clbuf));
  memset(hnbuf, 0, sizeof(hnbuf));
  (void)gethostname(hnbuf, sizeof(hnbuf));

#pragma omp parallel private(x, y, z, thread_id, coremask, clbuf)
  {
    thread_id = omp_get_thread_num();
    double t;

    t = MPI_Wtime();

    for (int i=0; i<niter; i++)              // main loop
      {
	x = cos(i*0.1)*exp(i*0.04);
	y = sin(i*0.1)*exp(i*0.04);
	z = ((x*x)+(y*y));  
      }
    
    t = MPI_Wtime() - t;

    sched_getaffinity(0, sizeof(coremask), &coremask);
    cpuset_to_cstr(&coremask, clbuf);

#pragma omp barrier

    printf("Rank %03d thread %02d on %s. core = %s (%f seconds).\n",
	   rank, thread_id, hnbuf, clbuf, t);
  }

  MPI_Finalize();
  return(0);
}


