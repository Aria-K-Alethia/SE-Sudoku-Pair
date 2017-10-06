#pragma once
#include "Sudoku.h"

class OutputHandler {
public:
    OutputHandler(Sudoku* sudoku);
    void outputSudoku(int count, int result[][LEN*LEN], char* filename);
    ~OutputHandler();
private:
    Sudoku* sudoku;
};

