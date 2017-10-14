#include "stdafx.h"
#include "InputHandler.h"
#include "Sudoku.h"
#include "OutputHandler.h"
#include "Output.h"
using namespace std;

#define MAXSUDOKU 10000
#define LEN 9

/*


Sudoku sudoku;
const int length = 100;
int argc1;
char** argv1;


void test1() {
	int puzzle[81] = {
		5,1,2,3,4,0,7,8,9,
		7,8,9,5,1,2,3,4,6,
		3,4,6,7,8,9,5,1,2,
		2,5,1,0,3,4,9,6,7,
		6,9,7,2,5,1,8,3,4,
		8,3,4,6,9,7,2,5,0,
		1,2,5,4,7,3,6,9,8,
		9,6,8,0,2,5,4,7,3,
		4,7,3,9,6,8,1,2,5 };
	int solution[81] = {
		5,1,2,3,4,6,7,8,9,
		7,8,9,5,1,2,3,4,6,
		3,4,6,7,8,9,5,1,2,
		2,5,1,8,3,4,9,6,7,
		6,9,7,2,5,1,8,3,4,
		8,3,4,6,9,7,2,5,1,
		1,2,5,4,7,3,6,9,8,
		9,6,8,1,2,5,4,7,3,
		4,7,3,9,6,8,1,2,5
	};
	int temp[81];
	bool ret;
	ret = sudoku.solve(puzzle, temp);
	OutputHandler output(&sudoku);
	InputHandler input(0, nullptr);
	fstream testFile("test.txt", ios::out);
	output.outputSudoku(testFile);
	testFile.close();
	int board[LEN*LEN];
	testFile.open("test.txt", ios::in);
	input.getBoard(testFile, board);
}
void test2() {
	int result[10][81];
	int solution[81];
	int lower;
	int upper;
	//generate easy sudoku
	lower = 35;
	upper = 45;
	sudoku.generate(10, lower, upper, false, result);
	OutputHandler output(&sudoku);
	output.outputSudoku(10, result, "test.txt");
}
void test3() {
	int lower = 45;
	int upper = 55;
	int number = 10;
	int result[10][81];
	int solution[81];
	sudoku.generate(10, lower, upper, true, result);
	for (int i = 0; i < number; ++i) {
		int solutionNumber = sudoku.countSolutionNumber(result[i], 2);
		int count = 0;
		for (int j = 0; j < 81; ++j) {
			if (result[i][j] == 0) count++;
		}
	}
}
void test4() {
	int result[10][81];
	sudoku.generate(10, EASYMODE, result);
	for (int i = 0; i < 10; ++i) {
		int count = 0;
		for (int j = 0; j < 81; ++j) {
			if (result[i][j] == 0) count++;
		}
	}

	sudoku.generate(10, NORMALMODE, result);
	for (int i = 0; i < 10; ++i) {
		int count = 0;
		for (int j = 0; j < 81; ++j) {
			if (result[i][j] == 0) count++;
		}
	}

	sudoku.generate(10, HARDMODE, result);
	for (int i = 0; i < 10; ++i) {
		int count = 0;
		for (int j = 0; j < 81; ++j) {
			if (result[i][j] == 0) count++;
		}
	}
}
void test5() {
	int result[1][81];
	bool exceptionThrown = false;
	try { // Test first SudokuCountException
		sudoku.generate(-1, 1, result);
	}
	catch (SudokuCountException& e) {
		exceptionThrown = true;
		e.what();
	}

	//test ModeRangeException
	exceptionThrown = false;

	try {
		sudoku.generate(1, -1, result);
	}
	catch (ModeRangeException& e) {
		exceptionThrown = true;
		e.what();
	}
	exceptionThrown = false;
	try {
		sudoku.generate(100000, 20, 50, true, result);
	}
	catch (SudokuCountException& e) {
		exceptionThrown = true;
		e.what();
	}

	//test LowerUpperException,case 1
	exceptionThrown = false;
	try {
		sudoku.generate(1, 1, 50, true, result);
	}
	catch (LowerUpperException& e) {
		exceptionThrown = true;
		e.what();
	}
	//test LowerUpperException,case 2
	exceptionThrown = false;
	try {
		sudoku.generate(1, 20, 56, true, result);
	}
	catch (LowerUpperException& e) {
		exceptionThrown = true;
		e.what();
	}
	//test LowerUpperException,case 3
	exceptionThrown = false;
	try {
		sudoku.generate(1, 50, 1, true, result);
	}
	catch (LowerUpperException& e) {
		exceptionThrown = true;
		e.what();
	}
}
void test6() {
	InputHandler* input;
	strcpy_s(argv1[1], length, "-n");
	strcpy_s(argv1[2], length, "1000");
	strcpy_s(argv1[3], length, "-r");
	strcpy_s(argv1[4], length, "20~55");
	strcpy_s(argv1[5], length, "-u");
	argc1 = 6;
	input = new InputHandler(argc1, argv1);
	input->analyze();
	input->getMode();
	input->getNumber();
	input->getUpper();
	input->getLower();
	input->getUnique();
	delete input;
}
void test7() {
	InputHandler* input;
	strcpy_s(argv1[1], length, "-s");
	strcpy_s(argv1[2], length, "puzzle.txt");
	argc1 = 3;
	input = new InputHandler(argc1, argv1);
	input->analyze();
	input->getMode();
	input->getFileName();
	delete input;
}
void test8() {
	InputHandler* input;
	strcpy_s(argv1[1], length, "-c");
	strcpy_s(argv1[2], length, "1000");
	argc1 = 3;
	input = new InputHandler(argc1, argv1);
	input->analyze();
	input->getMode();
	input->getNumber();

	delete input;
}
void test9() {
	char* filename = "sudoku.txt";
	int board[LEN*LEN];
	int count = 0;
	sudoku.generateCompleteNAndOutput(1000, filename);
	fstream f(filename, ios::in);
	InputHandler input(argc1, argv1);

	//should have 1000 valid sudoku
	while (input.getBoard(f, board)) {
		count++;
		sudoku.convertToTwoDimension(board);
		sudoku.check();
	}
	f.close();
}
void test10() {
	int solution[81] = {
		5,1,2,3,4,6,7,8,9,
		7,8,9,5,1,2,3,4,6,
		3,4,6,7,8,9,5,1,2,
		2,5,1,8,3,4,9,6,7,
		6,9,7,2,5,1,8,3,4,
		8,3,4,6,9,7,2,5,1,
		1,2,5,4,7,3,6,9,8,
		9,6,8,1,2,5,4,7,3,
		4,7,3,9,6,8,1,2,5
	};
	sudoku.convertToTwoDimension(solution);
	int solution2[81];
	sudoku.convertToOneDimension(solution2);
}
void test11() {
	int solution1[81] = {
		5,1,2,3,4,6,7,8,9,
		7,8,9,5,1,2,3,4,6,
		3,4,6,7,8,9,5,1,2,
		2,5,1,8,3,4,9,6,7,
		6,9,7,2,5,1,8,3,4,
		8,3,4,6,9,7,2,5,1,
		1,2,5,4,7,3,6,9,8,
		9,6,8,1,2,5,4,7,3,
		4,7,3,9,6,8,1,2,5
	};
	int solution2[81] = {
		5,1,2,3,4,6,7,8,9,
		7,8,9,5,1,2,3,4,6,
		3,4,6,7,8,9,5,1,2,
		2,5,1,8,3,4,9,6,7,
		6,9,7,2,5,1,8,3,4,
		8,3,4,6,9,7,2,5,1,
		1,2,5,4,7,3,7,9,8,
		9,6,8,1,2,5,4,7,3,
		4,7,3,9,6,8,1,2,5
	};
	sudoku.convertToTwoDimension(solution1);
	sudoku.convertToTwoDimension(solution2);
}

void test12() {
	InputHandler* input;
	strcpy_s(argv1[1], length, "-n");
	strcpy_s(argv1[2], length, "1000");
	strcpy_s(argv1[3], length, "-m");
	strcpy_s(argv1[4], length, "2");
	argc1 = 4;
	input = new InputHandler(argc1, argv1);
	input->analyze();
	input->getHardness();
}
*/




int main(int argc, char* argv[]) {

	InputHandler input(argc,argv);
	input.analyze();
	Sudoku sudoku;
	OutputHandler outputHandler = OutputHandler(&sudoku);
	char* outFileName = "sudoku.txt";
	int result[MAXSUDOKU][LEN * LEN];
	int mode = input.getMode();
	int number = input.getNumber();
	char* inFileName = input.getFileName();
	int lower = input.getLower();
    int upper = input.getUpper();
	bool unique = input.getUnique();
	int hardness = input.getHardness();
    
    if (mode == 's') { // Handle mode -s
        fstream inFile;
        inFile.open(inFileName, ios::in);
		if (!inFile.is_open())Output::error(4);
		fstream outFile(outFileName, ios::out);
        int board[LEN*LEN];
		int solution[LEN*LEN];
		while (input.getBoard(inFile, board)) {
			if (!sudoku.solve(board, solution)) Output::error(6);
			outputHandler.outputSudoku(outFile);
		}
		outFile.close();
    } else if (mode == 'c') { // Handle mode -c
		sudoku.generateCompleteNAndOutput(number,outFileName);
	} else if (mode == 'n') { // Handle mode -n
        srand((unsigned)time(NULL));
		if (unique && lower > 0 && upper > 0) { // Handle mode -r -u under -n
			sudoku.generate(number, lower,
				upper, unique, result);
		} else if (unique && lower == 0 && upper == 0) { // Handle mode -u under -n
			int tempLower = rand() % 10 + 20;
			int tempUpper = tempLower + rand() % 20;
			sudoku.generate(number, 20, 55, unique, result);
		} else if (!unique && lower > 0 && upper > 0) { // Handle mode -r under -n
			sudoku.generate(number, lower,
				upper, unique, result);
        } else if (hardness != 0) { //Handle mode -m under -n
            sudoku.generate(number, hardness, result);
        }

		outputHandler.outputSudoku(number, result, outFileName);
	}
	return 0;

	/*
	argv1 = new char*[6];
	for (int i = 0; i < 6; i++)
	{
		argv1[i] = new char[100];
	}
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();
	test8();
	test9();
	test10();
	test11();
	test12();
	return 0;
	*/
}

