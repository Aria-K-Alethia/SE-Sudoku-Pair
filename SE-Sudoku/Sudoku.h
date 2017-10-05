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
	inline void trace_back_n(int i, int j, int n, fstream& file);
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
	void generate_output_n(int n, char* filename);
	void generate(int number, int mode, int result[][LEN*LEN]);
	void generateCompleteN(int number);
	void set(char b[][LEN + 1]);
	bool solve(int puzzle[], int solution[]);
	void convertToTwoDimension(int puzzle[]);
	void convertToOneDimension(int solution[]);
	bool check();
	bool check_pos(int i, int j);
	bool check_generate_pos(int i, int j, int k);
	bool check_solve_pos(int i, int j, int k);
};


