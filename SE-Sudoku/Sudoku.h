#pragma once
#include "stdafx.h"
#include "fstream"
#include "exception"
#include "cassert"
#include "time.h"
using namespace std;

/*
@overview:The Sudoku class a mutable.It has a sudoku board of size 9x9 and can generate or solve a sudoku
*/
#define LEN 9
#define RANDOMHOLES 0
#define UPDOWNHOLES 1
#define EASYMODE 1
#define NORMALMODE 2
#define HARDMODE 3

enum ModeRange {
	EASYLOWER = 35,
	EASYUPPER = 45,
	NORMALLOWER = 45,
	NORMALUPPER = 55,
	HARDLOWER = 55,
	HARDUPPER = 64
};

class Sudoku {
private:
	char board[LEN + 1][LEN + 1];
	void init();
	inline void traceBackN(int i, int j, int n, int **result);
	inline bool traceBackSolve(int i, int j);
	inline int getBlock(int i);
	static long int count;
	//below is the fast code
	static char* out;
	static long int out_pos;
public:
	Sudoku();
	Sudoku(char **b);
	Sudoku(Sudoku &b);
	char* toString();
    void generate(int number, int lower, int upper, bool unique, int result[][LEN*LEN]);
	void digHoles(int count, int mode, int lower, int upper, int result[][LEN*LEN]);
	void generate(int number, int mode, int result[][LEN*LEN]);
	void generateCompleteN(int number, int result[][LEN*LEN]);
	void generateCompleteNAndOutput(int number, char * filename);
	void traceBackWriteFile(int i, int j, int number, fstream &outFile);
	void set(char b[][LEN + 1]);
	int countSolutionNumber(int solution[],int bound);
	void traceBackCountSolution(int i, int j, int *solutionNumber, int bound);
	bool solve(int puzzle[], int solution[]);
	void convertToTwoDimension(int puzzle[]);
	void convertToOneDimension(int solution[]);
	bool check();
	bool checkPos(int i, int j);
	bool checkGeneratePos(int i, int j, int k);
	bool checkSolvePos(int i, int j, int k);
};

class IllegalLengthException :public exception {
public:
    const char* what() const throw();
};

class SudokuCountException :public exception {
public:
    const char* what() const throw();
};

class ResultRowTooFewException :public exception {
public:
    const char* what() const throw();
};

class LowerUpperException :public exception {
public:
    const char* what() const throw();
};

class ModeRangeException :public exception {
public:
    const char* what() const throw();
};
