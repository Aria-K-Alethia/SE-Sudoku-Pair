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
using namespace std;


int main(int argc, char* argv[])
{
    Sudoku sudoku = Sudoku();
    int lower = 45;
    int upper = 50;
    int number = 10;
    int result[10][81];
    int solution[81];
    sudoku.generate(10, lower, upper, true, result);
    for (int i = 0; i < number; ++i) {
        int solutionNumber = sudoku.countSolutionNumber(result[i], 2);
        int count = 0;
        for (int j = 0; j < 81; ++j) {
            if (result[i][j] == 0) count++;
        }
    }
	return 0;
}

