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
        } else if (hardness == 0 && !unique && lower == 0 && upper == 0) { //Handle mode -n without any other parameter
            int tempLower = rand() % 10 + 20;
            int tempUpper = tempLower + rand() % 20;
            sudoku.generate(number, tempLower, tempUpper, unique, result);
        }

		outputHandler.outputSudoku(number, result, outFileName);
	}
	return 0;
}

void getTwoDimensionArray(int D1, int D2, int** result) {
	result = new int*[D1];
	for (int i = 0; i < D2; ++i) {
		result[i] = new int[D2];
	}
}
