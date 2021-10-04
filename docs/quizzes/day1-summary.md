# Summary of day 1

Questions can be copy-pasted e.g. to HackMD.


1. How is OpenMP program executed

   A. Parallal program is run as a set of independent process
   B. Program starts with single thread, threads are then forked and joined
   C. A special launcher program is needed for starting the program 
   D. Same program can be run in serial and in parallel

   A.
   B.
   C.
   D.

Correct: B, D


2. Which of the following statements are true

   A. OpenMP threads communicate by sending and receiving messages
   B. OpenMP program can be run in distributed memory supercomputer
   C. Number of threads is always the same as number of CPU cores
   D. OpenMP programs are vulnerable to race conditions

   A.
   B.
   C.
   D.

Correct: D


3. How is data visibilty defined

   A. All the threads share always all the data
   B. By set of default rules
   C. By data sharing clauses
   D. Via calls to OpenMP API 

   A.
   B.
   C.
   D.

Correct: C


4. What is the outcome of the following code snippet when run with 4 threads
   ```fortran
  integer :: i, prod, vec1(6), vec2(6)

  vec1 = 2
  vec2 = 4

  prod = 0
!$omp parallel do
  do i = 1, 6
    prod = prod + vec1(i) * vec2(i)
  end do
!$omp end parallel do

  write(*,*) prod
  ```

   A. Code cannot be run with 4 threads as loop is not distributed evenly
   B. I do not know
   C. 0
   D. 48
  
   A.
   B.
   C.
   D.

Correct: B

Code has race condition and the result is thus not well defined

