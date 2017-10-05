#include "string"
#include "fstream"
#include "iostream"
#include "memory.h"
#include "stdafx.h"
#include "assert.h"
#include "Sudoku.h"
#include "Output.h"

#define START 2
#define LEN 9

using namespace std;
long int Sudoku::count = 0;
//below is fast code
char* Sudoku::out = new char[163 * 1000000 + 1];
long int Sudoku::out_pos = 0;
//fast code end
/*
@overview:this file implement the class Sudoku in Sudoku.h
*/

//Constructor
Sudoku::Sudoku()
{
	//@overview:init a board with 0 and START in board[1][1]
	init();
}

Sudoku::Sudoku(char **b)
{
	//@overview:init a board in terms of b
	assert(b != NULL);
	for (int i = 1; i <= LEN; ++i) {
		for (int j = 1; j <= LEN; ++j) {
			board[i][j] = b[i][j];
		}
	}
}
Sudoku::Sudoku(Sudoku &b)
{
	//@overview:copy constructor
	for (int i = 1; i <= LEN; ++i) {
		for (int j = 1; j <= LEN; ++j) {
			board[i][j] = b.board[i][j];
		}
	}
}
//main method,including generate and solve method.

void Sudoku::generate(int number, int mode, int result[][LEN*LEN]) {

}
void Sudoku::generateCompleteN(int number)
{
	//@overview:generate n sudoku 
	//do some prepare
	assert(Sudoku::count == 0);
	fstream file;
	file.open(filename, ios::out);
	if (file.is_open()) {
		//start from (1,2) since START has been filled
		trace_back_n(1, 2, n, file);
	}
	else
		Output::error(4);
	file.close();

}


bool Sudoku::solve(int puzzle[], int solution[]) {
	/*
	@overview:solve sudoku in puzzle.save the outcome to solution
	*/
	bool ret;
	convertToTwoDimension(puzzle);
	
	ret = trace_back_solve(1, 1);

	convertToOneDimension(solution);

	return ret;
}

void Sudoku::convertToTwoDimension(int puzzle[]) {
	for (int i = 1; i <= LEN; ++i) {
		for (int j = 1; j <= LEN; ++j) {
			board[i][j] = puzzle[(i - 1)*LEN + (j - 1)] + '0';
		}
	}
}

void Sudoku::convertToOneDimension(int solution[]) {
	for (int i = 1; i <= LEN; ++i) {
		for (int j = 1; j <= LEN; ++j) {
			solution[(i - 1)*LEN + (j - 1)] = board[i][j] - '0';
		}
	}
}

/*
void Sudoku::solve_and_output(InputHandler input, char* filename)
{
	
	fstream infile(input.get_filename(), ios::in);
	if (!infile.is_open()) Output::error(4);
	char board[LEN + 1][LEN + 1];
	while (input.get_board(infile, board)) {
		set(board);
		if (solve()) {
			
			char* outcome = toString();
			file << outcome;
			delete outcome;
			
			//below is fast code
			toString();
			//fast code end
		}
		else {
			infile.close();
			Output::error(6);
		}
	}
	//below is fast code
	Sudoku::out[Sudoku::out_pos] = '\0';
	fstream file(filename, ios::out | ios::app);
	file << Sudoku::out;
	delete[] Sudoku::out;
	//fast code end
	infile.close();
	file.close();
}
*/
//some useful method

void Sudoku::set(char b[][LEN + 1])
{
	//@overview:copy a board from b
	assert(b != NULL);
	for (int i = 1; i <= LEN; ++i) {
		for (int j = 1; j <= LEN; ++j) {
			board[i][j] = b[i][j];
		}
	}
}

/*
char* Sudoku::toString()
{
//@overview:turn the board into a standard string.
char*outcome = new char[200];
int pos=0;
for (int i = 1; i <= LEN ; ++i) {
for (int j = 1; j <= LEN ; ++j) {
outcome[pos++] = board[i][j];
if (j != LEN) outcome[pos++] = ' ';
}
outcome[pos++] = '\n';
}
outcome[pos++] = '\n';
outcome[pos] = '\0';
return outcome;
}
*/

//below is fast code
inline char* Sudoku::toString()
{
	for (int i = 1; i <= LEN; ++i) {
		for (int j = 1; j <= LEN; ++j) {
			Sudoku::out[Sudoku::out_pos++] = board[i][j];
			if (j != LEN) Sudoku::out[Sudoku::out_pos++] = ' ';
		}
		Sudoku::out[Sudoku::out_pos++] = '\n';
	}
	Sudoku::out[Sudoku::out_pos++] = '\n';
	return NULL;
}
//fast code end

bool Sudoku::check()
{
	//@overview:check if the whole sudoku is valid
	for (int i = 1; i <= LEN; ++i) {
		for (int j = 1; j <= LEN; ++j) {
			if (!check_pos(i, j)) return false;
		}
	}
	return true;
}

bool Sudoku::check_pos(int i, int j)
{
	//@overview:check the num in pos (i,j) is valid or not
	bool used[LEN + 1];
	memset(used, 0, sizeof(used));
	int num;
	//check row
	for (int k = 1; k <= LEN; ++k) {
		num = board[i][k] - '0';
		if (num == 0) return false;
		if (!used[num]) used[num] = true;
		else return false;
	}
	//check column
	memset(used, 0, sizeof(used));
	for (int k = 1; k <= LEN; ++k) {
		num = board[k][j] - '0';
		if (num == 0) return false;
		if (!used[num]) used[num] = true;
		else return false;
	}

	//check the 3x3 block
	memset(used, 0, sizeof(used));
	int row, col;
	row = get_block(i);
	col = get_block(j);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			num = board[row + i][col + j] - '0';
			if (num == 0) return false;
			if (!used[num]) used[num] = true;
			else return false;
		}
	}
	return true;
}


//private method
void Sudoku::init()
{
	//@overview:init  a board with START number
	for (int i = 0; i <= LEN; ++i) {
		for (int j = 0; j <= LEN; ++j) {
			board[i][j] = '0';
		}
	}
	board[1][1] = START + '0';
}

inline int Sudoku::get_block(int i)
{
	/*@overview:get the first row or col in the block of row or col i
	@param:
	*/
	return ((i - 1) / 3) * 3 + 1;
}

inline void Sudoku::trace_back_n(int i, int j, int n, fstream& file)
{
	//@overview:trace back method for generate_output_n method.
	if (i == 9 && j == 10) {
		if (Sudoku::count >= n) return;
		//if (!check()) Output::error(7);
		/*
		char* outcome = toString();
		file << outcome;
		delete[]outcome;
		*/

		Sudoku::count++;
		//below is a fast code,if recover,delete it
		toString();
		if (Sudoku::count == n) {
			Sudoku::out[Sudoku::out_pos] = '\0';
			file << Sudoku::out;
			delete[] Sudoku::out;
		}
		//fast code end
		return;
	}
	if (i != 9 && j == 10) {
		++i;
		j = 1;
	}

	for (int k = 1; k <= LEN; ++k) {
		if (Sudoku::count >= n) return;
		if (check_generate_pos(i, j, k)) {   //check if it is ok to set k on (i,j)
			board[i][j] = k + '0';
			trace_back_n(i, j + 1, n, file);    //if can,recur to next place
		}
	}
	board[i][j] = '0';
}

inline bool Sudoku::trace_back_solve(int i, int j)
{
	/*@overview:trace back function when solve sudoku
	@param:
	*/
	if (i == LEN && j == LEN + 1) return true;
	if (i != LEN && j == LEN + 1) {
		j = 1;
		++i;
	}
	if (board[i][j] != '0') return trace_back_solve(i, j + 1);
	bool outcome;
	for (int k = 1; k <= LEN; ++k) {
		if (check_solve_pos(i, j, k)) {
			board[i][j] = k + '0';
			outcome = trace_back_solve(i, j + 1);
			if (outcome) return true;
		}
	}
	board[i][j] = '0';
	return false;
}

bool Sudoku::check_generate_pos(int i, int j, int k)
{
	/*@overview:check if the board is valid when board[i][j] = k,this method omit 0 in the board
	@param:
	*/

	//check row
	for (int a = 1; a < j; ++a) {
		if (board[i][a] == k + '0') return false;
	}
	//check col
	for (int a = 1; a < i; ++a) {
		if (board[a][j] == k + '0') return false;
	}
	//check 3x3 block
	int row, col;
	row = get_block(i);
	col = get_block(j);

	for (int a = row; a <= i; ++a) {
		for (int b = col; b <= col + 2; ++b) {
			if (a == i && b == j) continue;
			if (board[a][b] == k + '0') return false;
		}
	}

	return true;
}

bool Sudoku::check_solve_pos(int i, int j, int k)
{
	/*@overview:check if k in (i,j) is valid when solving a sudoku
	@param:
	*/

	//check row
	for (int a = 1; a <= LEN; ++a) {
		if (board[i][a] == k + '0') return false;
	}
	//check col
	for (int a = 1; a <= LEN; ++a) {
		if (board[a][j] == k + '0') return false;
	}
	//check 3x3 block
	int row, col;
	row = get_block(i);
	col = get_block(j);

	for (int a = row; a <= row + 2; ++a) {
		for (int b = col; b <= col + 2; ++b) {
			if (board[a][b] == k + '0') return false;
		}
	}

	return true;
}