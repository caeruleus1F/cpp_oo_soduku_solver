#ifndef SOLVER_H
#define SOLVER_H
#include "SodukuSquare.h"
/******************************************************************************
*	Constructors:
*		Solver()
*			Initializes and assigns values to the dynamic array representing the 81
*			cells of the Sudoku sheet.
*	
*	Destructors:
*		~Solver()
*			Deallocates the dynamic array.
*	
*	Methods:
*		void Start ()
*			Driver method of the Solver class.
*		void sheetInput ()
*			Takes the initial sheet values from the user.
*		void displayPossibles ()
*			Debug method that displays all the possible numbers for each cell on 
*			the sheet.
*		void displaySheet ()
*			Displays the actual numbers in a 9x9 block. Unknowns are represented
*			by zeroes.
*		void updatePossibles (short row, short column, short iActualNumber)
*			When an actual number is deduced, this method removes it as a possible number
*			for the row, column, and subdivision it belongs to.
*		bool makePass ()
*			Scans each cell to determine if it has been solved and if there is only one
*			possible number that could be the definitive number of that cell.
*	
******************************************************************************/
class Solver {
private:
	SodukuSquare ** sheet; // holds the Sudoku sheet dynamic array.

public:
	Solver ();
	~Solver ();
	
	void Start ();
	void SheetInput ();
	void DisplayPossibles ();
	void DisplaySheet ();
	void UpdatePossibles (short row, short column, short iActualNumber);
	bool MakePass ();

};
#endif