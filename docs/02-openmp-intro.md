---
title:  Introduction to OpenMP
author: CSC Training
date:   2021
lang:   en
---


# Processes and threads

![](img/processes-threads.png)

<div class="column">

## Process

  - Independent execution units
  - Have their own state information and *own memory* address space

</div>
<div class="column">

## Thread

  - A single process may contain multiple threads
  - Have their own state information, but *share* the *same memory*
    address space

</div>


# Processes and threads

![](img/processes-threads-highlight-threads.png)

<div class="column">

## Process

  - Long-lived: spawned when parallel program started, killed when
    program is finished
  - Explicit communication between processes

</div>
<div class="column">

## Thread

  - Short-lived: created when entering a parallel region, destroyed
    (joined) when region ends
  - Communication through shared memory

</div>


# OpenMP {.section}


# What is OpenMP?

- A collection of _compiler directives_ and _library routines_ ,
  together with a _runtime system_, for
  **multi-threaded**, **shared-memory parallelization**
- Fortran 77/9X/03 and C/C++ are supported
- Latest version of the standard is 5.0 (November 2018)
    - Full support for accelerators (GPUs)
    - Support latest versions of C, C++ and Fortran
    - Support for a fully descriptive loop construct
    - and more
- Compiler support for 5.0 is still incomplete
- This course does not discuss any 5.0 specific features


# Why would you want to learn OpenMP?

- OpenMP parallelized program can be run on your many-core workstation or on a
  node of a cluster
- Enables one to parallelize one part of the program at a time
    - Get some speedup with a limited investment in time
    - Efficient and well scaling code still requires effort
- Serial and OpenMP versions can easily coexist
- Hybrid MPI+OpenMP programming


# Three components of OpenMP

- Compiler directives, i.e. language extensions
    - Expresses shared memory parallelization
    - Preceded by sentinel, can compile serial version

- Runtime library routines (Intel: libiomp5, GNU: libgomp)
    - Small number of library functions
    - Can be discarded in serial version via conditional compiling

- Environment variables
    - Specify the number of threads, thread affinity etc.


# OpenMP directives

- OpenMP directives consist of a *sentinel*, followed by the directive
  name and optional clauses
- C/C++:
```C
#pragma omp directive [clauses]
```
- Fortran:
```Fortran
!$omp directive [clauses]
```

- Directives are ignored when code is compiled without OpenMP support


# Compiling an OpenMP program

- Compilers that support OpenMP usually require an option (flag) that
  enables the feature
    - GNU: `-fopenmp`
    - Intel: `-qopenmp`
    - Cray: `-h omp`
        * OpenMP enabled by default, -h noomp disables
    - PGI: `-mp[=nonuma,align,allcores,bind]`


# Parallel construct

<div class="column">

- Defines a *parallel region*
    - C/C++:
    ```C
    #pragma omp parallel [clauses]
       structured block
    ```
    - Fortran:
    ```fortran
    !$omp parallel [clauses]
       structured block
    !$omp end parallel
    ```
- Prior to it only one thread (main)
- Creates a team of threads: main + workers
- Barrier at the end of the block

</div>
<div class="column">

SPMD: Single Program Multiple Data
![](img/omp-parallel.png)

</div>


# Example: "Hello world" with OpenMP

<div class="column">
```fortran
program omp_hello

   write(*,*) "Hello world! -main"
!$omp parallel
   write(*,*) ".. worker reporting for duty."
!$omp end parallel
   write(*,*) "Over and out! -main"

end program omp_hello
```
```bash
> gfortran -fopenmp omp_hello.F90 -o omp
> OMP_NUM_THREADS=3 ./omp
 Hello world! -main
 .. worker reporting for duty.
 .. worker reporting for duty.
 .. worker reporting for duty.
 Over and out! -main
```
</div>

<div class="column">
```c
#include <stdio.h>

int main(int argc, char* argv[])
{
  printf("Hello world! -main\n");
#pragma omp parallel
  {
    printf(".. worker reporting for duty.\n");
  }
  printf("Over and out! -main\n");
}
```
```bash
> gcc -fopenmp omp_hello.c -o omp
> OMP_NUM_THREADS=3 ./omp
Hello world! -main
.. worker reporting for duty.
.. worker reporting for duty.
.. worker reporting for duty.
Over and out! -main
```
</div>


# How to distribute work?

- Each thread executes the same code within the parallel region
- OpenMP provides several constructs for controlling work distribution
    - Loop construct
    - Single/Master construct
    - Sections construct
    - Task construct
    - Workshare construct (Fortran only)
- Thread ID can be queried and used for distributing work manually
  (similar to MPI rank)

# Loop construct

- Directive instructing compiler to share the work of a loop
    - Each thread executes only part of the loop

```C
#pragma omp for [clauses]
...
```
```fortran
!$omp do [clauses]
...
!$omp end do
```
- in C/C++ limited only to "canonical" for-loops. Iterator base loops are
  also possible in C++
- Construct must reside inside a parallel region
    - Combined construct with omp parallel: \
          `#pragma omp parallel for` / `!$omp parallel do`


# Loop construct

```fortran
!$omp parallel
!$omp do
  do i = 1, n
     z(i) = x(i) + y(i)
  end do
!$omp end do
!$omp end parallel
```

```c
#pragma omp parallel
{
  #pragma omp for
  for (i=0; i < n; i++)
    z[i] = x[i] + y[i];
}
```

# Workshare directive (Fortran only)

- In Fortran many array operations can be done conveniently with array
  syntax, *i.e.* without explicit loops
    - Array assignments, forall and where statements *etc.*
- The `workshare` directive divides the execution of the enclosed structured
  block into separate units of work, each of which is executed only once

```fortran
real :: a(n,n), b(n,n), c(n,n) d(n,n)
...
!$omp parallel
  !$omp workshare
     c = a * b
     d = a + b
  !$omp end workshare
!$omp end parallel
```

- Note that performance may be bad in some compilers, in particular with Intel

# Sections construct

- Sections construct enables parallel execution of independent tasks
- Each section is run by a single thread

<div class=column>
```C
#pragma omp sections [clauses]
{
  #pragma omp section
  task1()
  #pragma omp section
  task2()
  #pragma omp section
  task3()
}
```
</div>
<div class=column>
```fortran
!$omp sections [clauses]
  !$omp section
  call task1()
  !$omp section
  call task2()
  !$omp section
  call task3()
!$omp end sections
```
</div>

- If nested parallelism is supported, "tasks" can in principle contain
  parallel regions
    - Performance may not be optimal
- Often, OpenMP **task** construct is more suitable


# Summary

- OpenMP can be used with compiler directives
    - Ignored when code is build without OpenMP
- Threads are launched within **parallel** regions
- `for`/`do` loops can be parallelized easily with loop construct
