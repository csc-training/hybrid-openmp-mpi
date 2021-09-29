/*                    
 *  This file is part of Christian's OpenMP Task-parallel Sudoku Solver
 *  
 *  Copyright (C) 2013 by Christian Terboven <christian@terboven.com>
 *                                                                       
 *  This program is free software; you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

#include "SudokuBoard.h"


int found_sudokus = 0;


/**
  * Solve the Sudoku puzzle by finding and counting all solutions (if all are requested)
  */
bool solve(int x, int y, CSudokuBoard* sudoku, bool printAnyFoundSolution = true, bool findAllSolutions = true)
{
	if(x == sudoku->getFieldSize()) {                 // end of line
		y++;
		x = 0;
		if(y == sudoku->getFieldSize())           // end
			return true;
	}   
  
	if(sudoku->get(y, x) > 0)                         // field already set
		return solve(x+1, y, sudoku, printAnyFoundSolution, findAllSolutions);    // tackle next field
    
	for(int i = 1; i <= sudoku->getFieldSize(); i++) {            // try all numbers
		if(!sudoku->check(x, y, i)) {
			sudoku->set(y, x, i);                  // if number fits, set it
			if(solve(x+1, y, sudoku, printAnyFoundSolution, findAllSolutions)) {  // tackle next field
#pragma omp atomic
				found_sudokus++;
				sudoku->incrementSolutionCounter();     // solution found :-)
				if (printAnyFoundSolution) {
					std::cout << "The following is a valid solution:" << std::endl;
					sudoku->printBoard();
					std::cout << std::endl;
				}
				if (!findAllSolutions) {
					return true;                // return, as only one solution was asked for
				}
			}
		}
	}
	
	sudoku->set(y, x, 0);                            // no solution found, reset field
	return false;
}



/**
  * Solve the Sudoku puzzle in parallel by finding and counting all solutions
  */
bool solve_parallel(int x, int y, CSudokuBoard* sudoku, bool printAnyFoundSolution = true)
{
	if (x == sudoku->getFieldSize()) {                       // end of line
		y++;
		x = 0;
		if(y == sudoku->getFieldSize())                     // end
			return true;
	}   
  
	if (sudoku->get(y, x) > 0) {                         // field already set
		return solve_parallel(x+1, y, sudoku, printAnyFoundSolution);    // tackle next field
	}
    
#if USE_SEQUENTIAL_CUTOFF
	if ( y > 1 ) {
		return solve(x, y, sudoku, printAnyFoundSolution);
	}
#endif
	
	for (int i = 1; i <= sudoku->getFieldSize(); i++) {      // try all numbers
		if (!sudoku->check(x, y, i)) {
#pragma omp task firstprivate(i,x,y,sudoku)
			{
				CSudokuBoard new_sudoku(*sudoku);
				new_sudoku.set(y, x, i);                    // if number fits, set it
				
				if (solve_parallel(x+1, y, &new_sudoku, printAnyFoundSolution)) {  // tackle next field
#pragma omp atomic
					found_sudokus++;
	//				sudoku->incrementSolutionCounter();     // solution found :-)
					if (printAnyFoundSolution) {
						std::cout << "The following is a valid solution:" << std::endl;
						new_sudoku.printBoard();
						std::cout << std::endl;
					}
				}
			}
		}
	}
	
#pragma omp taskwait
	sudoku->set(y, x, 0);                            // no solution found, reset field
	return false;
}



/** @brief program entry point
 */
int main(int argc, char* argv[]) {
	// measure the time
	double t3, t4;
	int nthreads = 0;

	// expect three command line arguments: field size, block size, and input file
	if (argc != 4) {
		std::cout << "Usage: sudoku.exe <field size> <block size> <input filename>" << std::endl;
		std::cout << std::endl;
		return -1;
	}
	else {
		CSudokuBoard *sudoku1 = new CSudokuBoard(atoi(argv[1]), atoi(argv[2]));
		if (!sudoku1->loadFromFile(argv[3])) {
			std::cout << "There was an error reading a Sudoku template from " << argv[3] << std::endl;
			std::cout << std::endl;
			return -1;
		}

		// print the Sudoku board template
		std::cout << "Given Sudoku template" << std::endl;
		sudoku1->printBoard();

		// solve the Sudoku by finding (and printing) all solutions
		t3 = omp_get_wtime();
      #pragma omp parallel sections
		{
			solve_parallel(0, 0, sudoku1, false);
		}		
		t4 = omp_get_wtime();

#pragma omp parallel 
		{
#pragma omp master
			nthreads = omp_get_num_threads();
		}

		std::cout << std::endl;
		std::cout << "In total there were " << found_sudokus << " solutions found in parallel " << std::endl;
		std::cout << std::endl;
		
		delete sudoku1;
	}
	
	// print the time
	std::cout << "Parallel computation took " << t4 - t3 << " seconds (" 
			<< nthreads << " threads)." << std::endl << std::endl;

	return 0;
}
