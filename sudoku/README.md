# Parallel Sudoku solving with OpenMP tasks

The C++ code here solves a Sudoku with brute-force method, *i.e.*
by trying out all the possible solutions. For each empty cell, one tries to fill in
numbers, and if the number is valid, a new board is generated. For the new board, one moves to
next empty cell, and starts to fill numbers, create a new board etc. If no number can be put into 
the empty cell, the board is rejected, and one starts from the beginning with the next possible 
number. If one reaches the final cell, the solution is valid.

Problem can be parallelized by making each new board a task. Parallelize the code by adding 
appropriate OpenMP directives
