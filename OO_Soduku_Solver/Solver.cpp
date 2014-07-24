#include "Solver.h"
#include "SodukuSquare.h"
#include <iostream>
using namespace std;

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

void Solver::Start () {
	bool changesMade = true;

	sheetInput();
	displaySheet();

	while (changesMade == true) {
		cout << "\n---------------------\n\n";
		changesMade = makePass();

		if (changesMade == true)
			displaySheet();

		//displayPossibles();
	}
	char a;
	cin >> a;
	deleteSheet();
}

void Solver::sheetInput () {
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
	// take input from the user
	for (short i = 0; i < 9; ++i) {
		cout << "Row " << i + 1 << ": ";
		cin >> tempCharArray;
		for (short j = 0; j < 9; ++j) {
			tempNumber = tempCharArray[j] - '0';
			updatePossibles (i, j, tempNumber);
		}
	}
}

void Solver::displayPossibles () {
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

void Solver::displaySheet () {
	for (short i = 0; i < 9; ++i) {
		for (short j= 0; j < 9; ++j) {
			cout << sheet[i][j].actualNumber;
		}
		cout << endl;
	}
}

void Solver::updatePossibles (short row, short column, short iActualNumber) {
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
		//cout << "row ";
		// update possibilities for the input column
		for (short j = 0; j < 9; ++j) {
			if (j != row && (sheet[j][column].possibleNumbers & bitwiseNumber))
				sheet[j][column].possibleNumbers -= bitwiseNumber;
		}
		//cout << "column ";

		// update the subdivision's possibilities
		for (short i = 0; i < 3; ++i) {
			for (short j = 0; j < 3; ++j) {
				if ((row != subRow * 3 + i && column != subColumn * 3 + j) && (sheet[subRow * 3 + i][subColumn * 3 + j].possibleNumbers & bitwiseNumber)) {
					sheet[subRow * 3 + i][subColumn * 3 + j].possibleNumbers -= bitwiseNumber;
				}
			}
		}
		//cout << "sub" << endl;
	}
	else { // what happens when the iActualNumber is 0?

	}
}

bool Solver::makePass () {
	short bitwiseNumber;
	bool changesMade = false;

	for (short i = 0; i < 9; ++i) { // selects the row
		for (short j = 0; j < 9; ++j) { // selects the column
			if (sheet[i][j].actualNumber == 0) { // if there is no actual number yet
				short matches = 0, number = 0;

				for (short k = 0; k < 9; ++k) { // cycle through bitwise values
					bitwiseNumber = static_cast<short> (pow (2, k));

					if (sheet[i][j].possibleNumbers & bitwiseNumber) { // if one of the pnumbers matches
						++matches; // increment
						number = k + 1; // and store the decimal value
					}
				}

				if (matches == 1) { // if there is only one match
					updatePossibles (i, j, number); // apply the update
					changesMade = true;
				}
			}
		}
	}

	return changesMade;
}

void Solver::deleteSheet () {
	for (short i = 0; i < 9; ++i)
		delete [] sheet[i];

	delete [] sheet;	
}
