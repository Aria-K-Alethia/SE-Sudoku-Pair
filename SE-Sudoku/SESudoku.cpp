#include "stdafx.h"

using namespace std;
#define MAXSUDOKU 10000


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
		while (input.get_board(inFile, board)) {
			sudoku.solve(board, solution);
			outputHandler.outputSudoku(outFile);
		}
		outFile.close();
    }
	else if (mode == 'c') { // Handle mode -c
		sudoku.generateCompleteN_And_Output(number,outFileName);
	}
	else if (mode == 'n') { // Handle mode -n
        srand((unsigned)time(NULL));
		if (unique && lower > 0 && upper > 0) { // Handle mode -r -u under -n
			sudoku.generate(number, lower,
				upper, unique, result);
		}
		else if (unique && lower == 0 && upper == 0) { // Handle mode -u under -n
			int tempLower = rand() % 10 + 20;
			int tempUpper = tempLower + rand() % 20;
			sudoku.generate(number, 20, 55, unique, result);
		}
		else if (!unique && lower > 0 && upper > 0) { // Handle mode -r under -n
			sudoku.generate(number, lower,
				upper, unique, result);
        } else if (hardness != 0){ //Handle mode -m under -n
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
