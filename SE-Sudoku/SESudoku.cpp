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


int main(int argc, char* argv[])
{
	InputHandler input(argc,argv);
	input.analyze();
	Sudoku sudoku;
	OutputHandler outputHandler = OutputHandler(&sudoku);
	char* outFileName = "sudoku.txt";
    
    if (input.get_mode() == 's') {
        fstream inFile;
		fstream outFile;
		fstream outFile;
        inFile.open(input.get_filename, ios::in);
        int board[LEN*LEN];
		int solution[LEN*LEN];
		while (input.get_board(inFile, board)) {
			sudoku.solve(board, solution);
			outFile.open(outFileName, ios::out);
		}
        
        outputHandler.outputSudoku
    }
    
	return 0;
}

