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

#include "SudokuBoard.h"

#include <cstring>

CSudokuBoard::CSudokuBoard(int fsize, int bsize)
	: field_size(fsize), block_size(bsize), solutions(-1)
{
	field = new int[field_size*field_size];
}


CSudokuBoard::CSudokuBoard(const CSudokuBoard& other)
	: field_size(other.getFieldSize()), block_size(other.getBlockSize()), solutions(other.getNumSolutions())
{
	field = new int[field_size*field_size];
	std::memcpy(field, other.field, sizeof(int) * field_size*field_size);
}


CSudokuBoard::~CSudokuBoard(void)
{
	delete[] field;
}


bool CSudokuBoard::loadFromFile(char *filename)
{
  std::ifstream ifile(filename);
  
  if (!ifile) {
    std::cout << "There was an error opening the input file " << filename << std::endl;
    std::cout << std::endl;
    return false;
  }

  for (int i = 0; i < this->field_size; i++) {
    for (int j = 0; j < this->field_size; j++) {
	  ifile >> this->field[ACCESS(i,j)];
    }
  }

  return true;
}


void CSudokuBoard::printBoard()
{
	for(int i = 0; i < field_size; i++) {
		for(int j = 0; j < field_size; j++) {
			std::cout << std::setw(3) << 
				this->field[ACCESS(i,j)] 
				<< " ";
		}
		std::cout << std::endl;
	}
}


bool CSudokuBoard::check(int x, int y, int value)
{
	if(checkHorizontal(y, value))
		return true;
	if(checkVertical(x, value))
		return true;
	if(checkBox(x, y, value))
		return true;
	return false;
}


bool CSudokuBoard::checkHorizontal(int y, int value) {
	for(int i = 0; i < field_size; i++)
		if(field[ACCESS(y,i)] == value)
			return true;
	return false;
}


bool CSudokuBoard::checkVertical(int x, int value) {
	for(int i = 0; i < field_size; i++)
		if(field[ACCESS(i,x)] == value)
			return true;
	return false;
}


bool CSudokuBoard::checkBox(int x, int y, int value) {
	// find suitable box edge
	int x_box = (int)(x / block_size) * block_size;
	int y_box = (int)(y / block_size) * block_size;

	for(int i = y_box; i < y_box + block_size; i++)
		for(int j = x_box; j < x_box + block_size; j++)
			if(field[ACCESS(i,j)] == value)
				return true;
  
	return false;
}
