#pragma once
#include "stdafx.h"
#include "Sudoku.h"

class OutputHandler {
public:
    OutputHandler(Sudoku* sudoku);
    void outputSudoku(int count, int result[][LEN*LEN], char* filename);
	void outputSudoku(char * outFileName);
    ~OutputHandler();
private:
    Sudoku* sudoku;
};

