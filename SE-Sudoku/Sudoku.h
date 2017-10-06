#pragma once
#include "stdafx.h"
#include "fstream"
#include "string"
#include "InputHandler.h"
#define LEN 9

using namespace std;

/*
@overview:The Sudoku class a mutable.It has a sudoku board of size 9x9 and can generate or solve a sudoku
*/

class Sudoku
{
private:
	char board[LEN + 1][LEN + 1];
	void init();
	inline void trace_back_n(int i, int j, int n, int result[][LEN * LEN]);
	inline bool trace_back_solve(int i, int j);
	inline int get_block(int i);
	static long int count;
	//below is the fast code
	static char* out;
	static long int out_pos;
public:
	Sudoku();
	Sudoku(char **b);
	Sudoku(Sudoku &b);
	inline char* toString();
    void generate(int number, int lower, int upper, bool unique, int result[][LEN*LEN]);
	void digHoles(int count, int mode, int lower, int upper, int result[][LEN*LEN]);
	void generate(int number, int mode, int result[][LEN*LEN]);
	void generateCompleteN(int number, int result[][LEN*LEN]);
	void set(char b[][LEN + 1]);
	int countSolutionNumber(int solution[],int bound);
	void trace_back_count_solution(int i, int j, int* solutionNumber,int bound);
	bool solve(int puzzle[], int solution[]);
	void convertToTwoDimension(int puzzle[]);
	void convertToOneDimension(int solution[]);
	bool check();
	bool check_pos(int i, int j);
	bool check_generate_pos(int i, int j, int k);
	bool check_solve_pos(int i, int j, int k);
};


