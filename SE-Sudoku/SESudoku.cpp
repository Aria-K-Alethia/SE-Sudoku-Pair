#include "stdafx.h"
#include "iostream"
#include "fstream"
#include "string"
#include "memory.h"
#include "fstream"
#include "Sudoku.h"
#include "InputHandler.h"
#include "time.h"
#include "regex"
#include "sstream"
#include "Output.h"
#include "OutputHandler.h"
using namespace std;
#define MAXSUDOKU 1000000


int main(int argc, char* argv[])
{
	InputHandler input(argc,argv);
	input.analyze();
	Sudoku sudoku;
	OutputHandler outputHandler = OutputHandler(&sudoku);
	char* outFileName = "sudoku.txt";
	int result[MAXSUDOKU][LEN*LEN];

	int mode = input.get_mode();
	int number = input.get_number();
	char* inFileName = input.get_filename();
	int Lower = input.getLower();
	int Upper = 
	bool unique = input.getUnique();
	int hardness = input.getHardness();
    
    if (mode == 's') {
        fstream inFile;
        inFile.open(inFileName, ios::in);
        int board[LEN*LEN];
		int solution[LEN*LEN];
		while (input.get_board(inFile, board)) {
			sudoku.solve(board, solution);
			outputHandler.outputSudoku(outFileName);
		}
    }
	else if (mode == 'c') {
		sudoku.generateCompleteN(number,result);
		outputHandler.outputSudoku(input.get_number, result, outFileName);
	}
	else if (mode == 'n') {
		if (input.getUnique() && Lower() > 0 && input.getUpper() > 0) {
			sudoku.generate(input.get_number, Lower(),
				input.getUpper(), input.getUnique, result);
		}
		else if (input.getUnique() && Lower() == 0 && input.getUpper() == 0) {
			srand((unsigned)time(NULL));
			int lower = rand() % 10 + 20;
			int upper = lower + rand() % 20;
			sudoku.generate(number, lower, upper, input.getUnique(), result);
		}
		else if (!input.getUnique() && Lower() > 0 && input.getUpper() > 0) {
			sudoku.generate(input.get_number, Lower(),
				input.getUpper(), input.getUnique, result);
		}
		outputHandler.outputSudoku(input.get_number, result, outFileName);
	}

    
	return 0;
}

void getTwoDimensionArray(int D1, int D2, int** result) {
	result = new int*[D1];
	for (int i = 0; i < D2; ++i) {
		result[i] = new int[D2];
	}
}
