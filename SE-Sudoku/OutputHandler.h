#pragma once
#include <fstream>
#include "Sudoku.h"
#define LEN 9
using namespace std;
//class Sudoku;
class OutputHandler;


class OutputHandler {
public:
	OutputHandler(Sudoku* sudoku);
    void outputSudoku(int count, int result[][LEN*LEN], char* filename);
	void outputSudoku(fstream& outFile);
    ~OutputHandler();
private:
    Sudoku* sudoku;
};

