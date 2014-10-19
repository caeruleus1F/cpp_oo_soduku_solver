#ifndef SODUKUSQUARE_H
#define SODUKUSQUARE_H
/******************************************************************************
*	Represents a cell on the Sudoku sheet. Contains two short datamembers that hold
*	the possible numbers (in bitwise form) and the actual number.
*	
*	Data members:
*	short possibleNumbers
*	short actualNumber
******************************************************************************/
struct SodukuSquare {
	short possibleNumbers; // holds the possible numbers in bitwise form. bits 0 through 8 represent 1 through 9.
	short actualNumber; // holds the definitive number that remains after all other possible numbers have been ruled out.
};
#endif