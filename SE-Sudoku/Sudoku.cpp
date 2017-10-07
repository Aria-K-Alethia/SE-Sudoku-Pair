#include "stdafx.h"
#include "Sudoku.h"

using namespace std;
long int Sudoku::count = 0;

/*
@overview:this file implement the class Sudoku in Sudoku.h
*/

//Constructor
Sudoku::Sudoku() {
	//@overview:init a board with 0 and START in board[1][1]
	init();
}

Sudoku::Sudoku(char **b) {
	//@overview:init a board in terms of b
	assert(b != NULL);
	for (int i = 1; i <= LEN; ++i) {
		for (int j = 1; j <= LEN; ++j) {
			board[i][j] = b[i][j];
		}
	}
}
Sudoku::Sudoku(Sudoku &b) {
	//@overview:copy constructor
	for (int i = 1; i <= LEN; ++i) {
		for (int j = 1; j <= LEN; ++j) {
			board[i][j] = b.board[i][j];
		}
	}
}
//main method,including generate and solve method.

void Sudoku::generate(int number, int lower, int upper, bool unique, int result[][LEN*LEN]) throw(SudokuCountException, 
    //ResultRowTooFewException, 
	LowerUpperException) {
	//@overview: generate puzzles with restriction of count, uniqueness and hole range

	if (number > 10000 || number < 1) {
		throw SudokuCountException();
	}
	/*if (number > sizeof(result) / (81 * sizeof(int))) {
		throw ResultRowTooFewException();
	}*/
	if (lower < 20 || upper > 55 || lower > upper) {
		throw LowerUpperException();
	}


	generateCompleteN(number, result);
	if (unique) {
		digHoles(number, UPDOWNHOLES, lower, upper, result);
    } else {
		digHoles(number, RANDOMHOLES, lower, upper, result);
    }
}

void Sudoku::digHoles(int count, int mode, int lower, int upper, int result[][LEN*LEN]) {
	//@overview: dig holes with choice of mode, randomly or sequently
	int seed = (unsigned int)time(NULL);
	srand(seed);
	if (mode == RANDOMHOLES) { //Dig holes randomly
        for (int i = 0; i < count; i++) {
			int holeCount = rand() % (upper - lower) + lower; //Select a hole count between lower & upper
			for (int j = 0; j < holeCount;)
			{
				int location = rand() % 81;
				if (result[i][location] != 0)
				{
					result[i][location] = 0;
					j++; // New hole dug, go to next hole
				}
			}
        }
    } else if (mode == UPDOWNHOLES) { //Dig holes from left to right, from up to down, unique answer restricted
		for (int i = 0; i < count; i++) {
			int dugCount = 0;
			int holeCount = rand() % (upper - lower) + lower;
			for (int j = 0; j < LEN*LEN; j++) {
				if (j == 0) { // The first digging can always be right
                    result[i][j] = 0;
					dugCount++;
					if (dugCount == holeCount) {
						break;
					}
					continue; 
				}
				int temp = result[i][j];
				int k;
				for (k = 1; k <= LEN; k++) { //Try replace the number with other numbers and solve
					if (k == temp) {
						continue;
					}
					result[i][j] = k; // Replace the original number with others from 1 to 9
					int tempSudoku[LEN*LEN];
					if (solve(result[i], tempSudoku)) { //Can find two solutions with this hole
						result[i][j] = temp; // Can't dig, recover
						break;
					}
				}
				if (k > LEN) {
					result[i][j] = 0;
					dugCount++;
				}
				if (dugCount == holeCount) {
					break;
				}
			}
		}
    }
    
}

void Sudoku::generate(int number, int mode, int result[][LEN*LEN]) throw(SudokuCountException,
    //ResultRowTooFewException,
	 ModeRangeException){
    //@overview: generate puzzles with restriction of difficulty level

	if (number > 10000 || number < 1) {
		throw SudokuCountException();
	}
	/*if (number > sizeof(result) / (LEN * LEN * sizeof(int))) {
		throw ResultRowTooFewException();
	}*/
	if (mode < EASYMODE || mode > HARDMODE) {
		throw ModeRangeException();
	}
    switch (mode) {
    case EASYMODE:
        generate(number, EASYLOWER, EASYUPPER, false, result);
        break;
    case NORMALMODE:
        generate(number, NORMALLOWER, NORMALUPPER, false, result);
        break;
    case HARDMODE:
        generate(number, HARDLOWER, HARDUPPER, false, result);
        break;
    default:
        break;
    }

}

void Sudoku::generateCompleteN(int number, int result[][LEN * LEN]) {
	//@overview:generate n sudoku 
	//do some prepare
	init();
	Sudoku::count = 0;
    traceBackN(1, 1, number, result);
	
}

void Sudoku::generateCompleteNAndOutput(int number, char* filename) {
	/*
		@overview:generate number complete sudoku for -c in file with filename
	*/
	fstream outFile(filename, ios::out);
	Sudoku::count = 0;
    traceBackWriteFile(1, 1, number, outFile);
	outFile.close();
}

void Sudoku::traceBackWriteFile(int i, int j, int number, fstream &outFile) {
	/*
		@overview:trace back method for generateCompleteN_And_Output
	*/
	if (i == 9 && j == 10) {
		Sudoku::count++;
		outFile << toString();
		return;
	}
	if (i != 9 && j == 10) {
		++i;
		j = 1;
	}

	for (int k = 1; k <= LEN; ++k) {
		if (Sudoku::count >= number) return;
		if (checkGeneratePos(i, j, k)) {   //check if it is ok to set k on (i,j)
			board[i][j] = k + '0';
            traceBackWriteFile(i, j + 1, number, outFile);    //if can,recur to next place
		}
	}
	board[i][j] = '0';
}

bool Sudoku::solve(int puzzle[], int solution[]) throw(IllegalLengthException) {
	/*
	@overview:solve sudoku in puzzle.save the outcome to solution
	*/
	bool ret;
	/*if ((sizeof(puzzle)/sizeof(int) - LEN*LEN != 0)|| (sizeof(solution)/sizeof(int) - LEN*LEN != 0)) {
		throw IllegalLengthException();
	}
	*/

	convertToTwoDimension(puzzle);
	
	ret = traceBackSolve(1, 1);

    if (!check()) { // The sudoku is illegal
        return false;
    }

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
	
	fstream infile(input.getFileName(), ios::in);
	if (!infile.is_open()) Output::error(4);
	char board[LEN + 1][LEN + 1];
	while (input.getBoard(infile, board)) {
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

/*
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
*/
bool Sudoku::check()
{
	//@overview:check if the whole sudoku is valid
	for (int i = 1; i <= LEN; ++i) {
		for (int j = 1; j <= LEN; ++j) {
			if (!checkPos(i, j)) return false;
		}
	}
	return true;
}

bool Sudoku::checkPos(int i, int j)
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
	row = getBlock(i);
	col = getBlock(j);
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

int Sudoku::countSolutionNumber(int puzzle[],int bound) {
	/*
	@overview:count the solution number in puzzle and return it
	*/
	convertToTwoDimension(puzzle);
	int solutionNumber = 0;
    traceBackCountSolution(1, 1, &solutionNumber, bound);
	return solutionNumber;
}

void Sudoku::traceBackCountSolution(int i, int j, int *solutionNumber, int bound) {
	/*
	@overview:trace back method for countSolutionNumber,save the solution
	number in the pointer solutionNumber
	*/
	if (i == LEN && j == LEN + 1) {
		(*solutionNumber)++;
		return;
	}
	if (i != LEN && j == LEN + 1) {
		i++;
		j = 1;
	}
    if (board[i][j] != '0') {
        traceBackCountSolution(i, j + 1, solutionNumber, bound);
        return;
    }
	for (int k = 1; k <= LEN; k++) {
		if (checkSolvePos(i, j, k)) {
			board[i][j] = k + '0';
            traceBackCountSolution(i, j + 1, solutionNumber, bound);
			if (*solutionNumber >= bound) return;
		}
	}
	board[i][j] = '0';
	return;
}

//private method
void Sudoku::init() {
	//@overview:init  a board with START number
	for (int i = 0; i <= LEN; ++i) {
		for (int j = 0; j <= LEN; ++j) {
			board[i][j] = '0';
		}
	}
}

inline int Sudoku::getBlock(int i) {
	/*@overview:get the first row or col in the block of row or col i
	@param:
	*/
	return ((i - 1) / 3) * 3 + 1;
}

inline void Sudoku::traceBackN(int i, int j, int n, int result[][LEN*LEN]) {
	//@overview:trace back method for generate_output_n method.
	if (i == 9 && j == 10) {
        Sudoku::convertToOneDimension(result[Sudoku::count]);
		Sudoku::count++;
		//below is a fast code,if recover,delete it
		if (Sudoku::count == n) {
            return;
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
		if (checkGeneratePos(i, j, k)) {   //check if it is ok to set k on (i,j)
			board[i][j] = k + '0';
            traceBackN(i, j + 1, n, result);    //if can,recur to next place
		}
	}
	board[i][j] = '0';
}

inline bool Sudoku::traceBackSolve(int i, int j) {
	/*@overview:trace back function when solve sudoku
	@param:
	*/
	if (i == LEN && j == LEN + 1) return true;
	if (i != LEN && j == LEN + 1) {
		j = 1;
		++i;
	}
	if (board[i][j] != '0') {
		return traceBackSolve(i, j + 1);
	}
	bool outcome;
	for (int k = 1; k <= LEN; ++k) {
		if (checkSolvePos(i, j, k)) {
			board[i][j] = k + '0';
			outcome = traceBackSolve(i, j + 1);
			if (outcome) return true;
		}
	}
	board[i][j] = '0';
	return false;
}

bool Sudoku::checkGeneratePos(int i, int j, int k) {
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
	row = getBlock(i);
	col = getBlock(j);

	for (int a = row; a <= i; ++a) {
		for (int b = col; b <= col + 2; ++b) {
			if (a == i && b == j) continue;
			if (board[a][b] == k + '0') return false;
		}
	}

	return true;
}

bool Sudoku::checkSolvePos(int i, int j, int k) {
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
	row = getBlock(i);
	col = getBlock(j);

	for (int a = row; a <= row + 2; ++a) {
		for (int b = col; b <= col + 2; ++b) {
			if (board[a][b] == k + '0') return false;
		}
	}

	return true;
}

char Sudoku::getElem(int i, int j) {
	return board[i][j];
}

const char * IllegalLengthException::what() const throw() {
    return "IllegalLengthException: The length of puzzle or solution in solve(puzzle,solution) must be 81";
}

const char * SudokuCountException::what() const throw() {
    return "SudokuCountException: Number in generate(int number,int lower,int upper,bool unique,int result[][]) must in[1,10000]";
}

const char * ResultRowTooFewException::what() const throw() {
    return "ResultRowTooFewException: The row of result array must be greater than number";
}

const char * LowerUpperException::what() const throw() {
    return "LowerUpperException: The lower and upper in generate(int number,int lower,int upper,bool unique,int result[][]) must satisfy:lower<upper,lower > 20,upper < 55";
}

const char * ModeRangeException::what() const throw() {
    return "ModeRangeException: The number of mode must in [1,3]";
}
