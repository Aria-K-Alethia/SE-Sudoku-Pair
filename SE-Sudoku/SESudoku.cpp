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
    
    if (input.get_mode() == 's') {
        fstream inFile;
        inFile.open(input.get_filename, ios::in);
        int board[LEN*LEN];
        input.get_board(inFile, board);
        Sudoku sudoku;
        int solution[LEN*LEN];
        sudoku.solve(board, solution);
        fstream outFile;
        outFile.open("sudoku.txt", ios::out);
        OutputHandler outputHandler = OutputHandler(&sudoku);
        outputHandler.outputSudoku
    }
    
	return 0;
}

