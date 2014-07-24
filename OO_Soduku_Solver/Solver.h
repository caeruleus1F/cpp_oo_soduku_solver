#ifndef SOLVER_H
#define SOLVER_H
#include "SodukuSquare.h"

class Solver {
private:
	SodukuSquare ** sheet;
public:
	Solver ();
	void Start ();
	void sheetInput ();
	void displayPossibles ();
	void displaySheet ();
	void updatePossibles (short row, short column, short iActualNumber);
	bool makePass ();
	void deleteSheet ();
};
#endif