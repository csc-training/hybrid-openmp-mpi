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
#include <iomanip>
#include <fstream>
#include <vector>

#define ACCESS(x, y) (this->field_size*(x) + (y))

/**
 * @brief Representation of the Sudoku board, includes solver and utility functions
 */
class CSudokuBoard
{
public:
	CSudokuBoard(int fsize, int bsize);
	CSudokuBoard(const CSudokuBoard& other);
	~CSudokuBoard(void);

	inline int getNumSolutions() const {
		return this->solutions;
	}
	
	inline void incrementSolutionCounter() {
		if (this->solutions == -1)
			this->solutions = 1;
		else
			this->solutions++;
	}
	
	inline int getFieldSize() const {
		return this->field_size;
	}
	
	inline int getBlockSize() const {
		return this->block_size;
	}
	
	inline int get(int x, int y) const {
		return this->field[ACCESS(x,y)];
	}
	
	inline void set(int x, int y, int value) {
		this->field[ACCESS(x, y)] = value;
	}

	/**
	 * Read Sudoku template from file
	 * @param filename name of file to read input from
	 * @return true if reading file was successful, false if not
	 */
	bool loadFromFile(char *filename);

	/**
	 * Print the Sudoku board to stdout
	 */
	void printBoard();

	/**
	 * Check whether value already used
	 * @return true if already used, false if not
	 */
	bool check(int x, int y, int value);


private:
	bool checkHorizontal(int y, int value);
	bool checkVertical(int x, int value);
	bool checkBox(int x, int y, int value);
	
	int field_size;
	int block_size;

	int *field;

	int solutions;
};
