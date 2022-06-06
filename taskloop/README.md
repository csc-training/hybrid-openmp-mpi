# Tasks, loops and reductions

The skeleton code [sum.c](sum.c) / [sum.F90](sum.F90) makes a sum over two different arrays.
Parallelize the code so that sum over each array is an OpenMP task. Parallelize also
the `for` / `do` loops. What kind of workshare constructs it is possible to use? Play around with
`omp for` / `omp do` and `taskloop`. Do you need some additional clauses for getting the same 
correct result as serially? 
