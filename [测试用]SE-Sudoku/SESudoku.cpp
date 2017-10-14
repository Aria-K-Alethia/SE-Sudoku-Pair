#include "stdafx.h"
#include "InputHandler.h"
#include "Sudoku.h"
#include "OutputHandler.h"
#include "Output.h"
using namespace std;

#define MAXSUDOKU 10000
#define LEN 9


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

