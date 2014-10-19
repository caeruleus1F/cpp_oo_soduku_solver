#include "Solver.h"
#include "SodukuSquare.h"
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

/******************************************************************************
*	PURPOSE: Initializes and assigns starting values for the dynamic array.
*	
*	ENTRY: On instantiation of the Solver class in Main().
*	
*	EXIT: After initial values have been assigned. The actual number is set to
*	zero to represent an unsolved state. Datamember possibleNumbers is set to
*	511 which represents the sum of 2^9 + ... 2^0. Should an array have been
*	used instead? Maybe, but I had just learned about bitwise operations at
*	the creation of this program and wanted to try something new.
*	
******************************************************************************/
Solver::Solver () {
	sheet = new SodukuSquare * [9]();

	for (int i = 0; i < 9; ++i) {
		sheet[i] = new SodukuSquare [9]();

		for (int j = 0; j < 9; ++j) {
			sheet[i][j].actualNumber = 0;
			sheet[i][j].possibleNumbers = 511; // this number is the sum of the first 9 bits
		}
	}
}

/******************************************************************************
*	PURPOSE: Deallocates the dynamic array on destruction of the Solver class.
*	
*	ENTRY: After the Sudoku puzzle is solved or no more changes can be made.
*	
*	EXIT: After the sheet has been deallocated.
*	
******************************************************************************/
Solver::~Solver () {
	for (short i = 0; i < 9; ++i)
		delete[] sheet[i];

	delete[] sheet;
}

/******************************************************************************
*	PURPOSE: Driver method of the Solver class.
*	
*	ENTRY: After instantiation, call this method to begin the process of
*	solving the Sudoku puzzle.
*	
*	EXIT: Calls for the user to input initial values into the sheet. Makes a
*	pass on each loop that determines if any actual numbers were solved. If
*	so, then another pass will be made. If no new actual numbers were
*	determined, then either the sheet has been solved, or it's too complex for
*	the current problem-solving capability of the program. After each
*	iteration where changes are made, the sheet is displayed.
*	
******************************************************************************/
void Solver::Start () {
	bool changesMade = true;

	SheetInput();
	DisplaySheet();

	while (changesMade == true) {
		cout << "\n---------------------\n\n";
		changesMade = MakePass();

		if (changesMade == true)
			DisplaySheet();

		//DisplayPossibles();
	}
	char a;
	cin >> a;
}

/******************************************************************************
*	PURPOSE: Takes input from the user.
*	
*	ENTRY: Called from the Start() member method.
*	
*	EXIT: Prompts the user to enter in each row at a time. The nine numbers
*	are taken in as a character array and then implicitly typecast to integers
*	which are then fed into updatePossibles one at a time. Entering a value of
*	zero means that particular cell is unsolved.
*	
******************************************************************************/
void Solver::SheetInput () {
	char tempCharArray [10] = {"\0"};
	short tempNumber = 0;
	//int testSheet [9][9] = { 
	//	{ 9, 0, 0, 6, 0, 0, 2, 0, 0 }, { 0, 3, 2, 4, 8, 0, 0, 9, 5 }, { 5, 0, 1, 0, 3, 0, 0, 6, 0 },
	//	{ 2, 0, 4, 0, 0, 0, 0, 0, 0 }, { 7, 0, 0, 1, 5, 4, 0, 0, 9 }, { 0, 0, 0, 0, 0, 0, 5, 0, 6 },
	//	{ 0, 7, 0, 0, 2, 0, 9, 0, 4 }, { 4, 1, 0, 0, 6, 8, 3, 5, 0 }, { 0, 0, 5, 0, 0, 1, 0, 0, 8 }
	//};
	//
	//for (int i = 0; i < 9; ++i) {
	//	for (int j = 0; j < 9; ++j) {
	//		//cout << i << "-" << j << endl;
	//		updatePossibles (i, j, testSheet[i][j]);
	//	}
	//}
	
	cout << "Enter initial values (0 for blank) ---" << endl;
	
	for (short i = 0; i < 9; ++i) {
		cout << "Row " << i + 1 << ": ";
		cin >> tempCharArray;
		for (short j = 0; j < 9; ++j) {
			tempNumber = tempCharArray[j] - '0';
			UpdatePossibles (i, j, tempNumber);
		}
	}
}

/******************************************************************************
*	PURPOSE: Debug method used to output all of the possible numbers for each
*	of the 81 cells contained in the dynamic array.
*	
*	ENTRY: Called from Start() if it's been uncommented out.
*	
*	EXIT: Outputs all of the possible numbers for for each cell in the sheet
*	from left to right, top to bottom. Cells which have been solved will only
*	have one number displayed, while unsolved cells can have 2 to 9 displayed
*	numbers, seperated by comma.
*	
******************************************************************************/
void Solver::DisplayPossibles () {
	for (short i = 0; i < 9; ++i) {
		for (short j= 0; j < 9; ++j) {
			cout << i + 1 << "-";
			cout << j + 1 << ": ";
			for (short k = 0; k < 9; ++k) {
				if (sheet[i][j].possibleNumbers & static_cast<short> (pow (2, k))) {
					cout << k + 1 << ", ";
				}
			}
			cout << endl;
		}
	}
}

/******************************************************************************
*	PURPOSE: Displays sheet's actualNumbers in a 9x9 block. Non-zero numbers
*	are the definitive number for that cell, while a zero represents an
*	unsolved state.
*	
*	ENTRY: Called from the Start() member function.
*	
*	EXIT: The actualNumbers are displayed in a 9x9 block from nested FOR
*	loops.
*	
******************************************************************************/
void Solver::DisplaySheet () {
	for (short i = 0; i < 9; ++i) {
		for (short j= 0; j < 9; ++j) {
			cout << sheet[i][j].actualNumber;
		}
		cout << endl;
	}
}

/******************************************************************************
*	PURPOSE: Removes the input number (third argument) from the list of
*	possible numbers from each cell in its row, column, and subdivision.
*	
*	ENTRY: Called from sheetInput() and makePass().
*	
*	EXIT: If iActualNumber is not zero, then it is the definitive number for
*	the cell. The bitwise representation of that number is determined. The
*	cell that is associated with the definitive number is assigned that
*	number and its bitwise representation, signifying that it has been solved.
*	The remaining cells that are in the same row, column, and subdivision are
*	then updated by subtracting the bitwise value from their respective
*	possibleNumbers datamember - checking to make sure that that value is
*	still considered to be a possible number, otherwise things could get
*	screwy.
*	
******************************************************************************/
void Solver::UpdatePossibles (short row, short column, short iActualNumber) {
	if (iActualNumber != 0) {
		short subRow = row / 3, subColumn = column / 3;
		short bitwiseNumber = static_cast<short> (pow (2, iActualNumber - 1));

		sheet[row][column].actualNumber = iActualNumber;
		sheet[row][column].possibleNumbers = bitwiseNumber;

		// update possibilities for the input row
		for (short i = 0; i < 9; ++i) {
			// if the selected row and column are not the same
			// and one of the possible numbers is also the input 
			// actual number, then subtract the actual number
			// from the list of possible numbers at that cell.
			if (i != column && (sheet[row][i].possibleNumbers & bitwiseNumber))
				sheet[row][i].possibleNumbers -= bitwiseNumber;
		}

		// update possibilities for the input column
		for (short j = 0; j < 9; ++j) {
			if (j != row && (sheet[j][column].possibleNumbers & bitwiseNumber))
				sheet[j][column].possibleNumbers -= bitwiseNumber;
		}

		// update the subdivision's possibilities
		for (short i = 0; i < 3; ++i) {
			for (short j = 0; j < 3; ++j) {
				if ((row != subRow * 3 + i &&  column != subColumn * 3 + j) && 
					(sheet[subRow * 3 + i][subColumn * 3 + j].possibleNumbers & bitwiseNumber)) {
					sheet[subRow * 3 + i][subColumn * 3 + j].possibleNumbers -= bitwiseNumber;
				}
			}
		}
	}
	else {

	}
}

/******************************************************************************
*	PURPOSE: Iterates through the dynamic array looking for unsolved cells
*	within the sheet.
*	
*	ENTRY: Called from Start().
*	
*	EXIT: A cell is determined to be unsolved by having two or more bitwise 
*	values - or however that works exactly. Each time the possibleNumbers 
*	datamember for that cell is checked using bitwise operations and returns 
*	true, a counter (matches) is incremented. If only one match for that
*	cell has been found and the acutalNumber for that cell is zero, then
*	it has been newly solved and is then passed into the updatePossibles
*	method to make sure no other cell in its row, column, and subdivision can
*	resolve to that value and probably cause a fail-cascade in the process.
*	If changes were made during this pass, then the sheet is probably not
*	solved, which means this method will be called again. If no changes were
*	made, then the sheet is either solved or is unsolveable because each of
*	the remaining unsolved cells contains two or more possible numbers. In
*	order to solve the puzzle, a duplicate sheets would have to be made that
*	use the remaining possibleNumbers from the unsolved cells as actualNumbers
*	and attempts to solve. This could be very memory intensive at higher
*	difficulties if not implemented efficiently, since a blank sheet can
*	have up to multiple billion solutions, which translates into over
*	81-multiple billion gigabytes of memory needed. Even solving that many
*	puzzles would require the user to leave their computer on overnight.
*	
******************************************************************************/
bool Solver::MakePass () {
	short bitwiseNumber;
	bool changesMade = false;

	for (short i = 0; i < 9; ++i) { // selects the row
		for (short j = 0; j < 9; ++j) { // selects the column
			if (sheet[i][j].actualNumber == 0) { // if there is no actual number yet
				short matches = 0, number = 0;

				for (short k = 0; k < 9; ++k) { // cycle through bitwise values
					bitwiseNumber = static_cast<short> (pow (2, k));

					if (sheet[i][j].possibleNumbers & bitwiseNumber) {
						++matches; // increment
						number = k + 1; // and store the value
					}
				}

				if (matches == 1) { // if there is only one match
					UpdatePossibles (i, j, number); // apply the update
					changesMade = true;
				}
			}
		}
	}

	return changesMade;
}