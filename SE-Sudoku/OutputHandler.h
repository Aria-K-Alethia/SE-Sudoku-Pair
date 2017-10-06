#pragma once
#include "stdafx.h"


class OutputHandler {
public:
    OutputHandler(Sudoku* sudoku);
    void outputSudoku(int count, int result[][LEN*LEN], char* filename);
	void outputSudoku(fstream& outFile);
    ~OutputHandler();
private:
    Sudoku* sudoku;
};

