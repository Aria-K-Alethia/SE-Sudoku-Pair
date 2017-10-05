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
	/*test performance
	clock_t on, off;
	on = clock();
	*/
	InputHandler input(argc, argv);
	char* filename = "sudoku.txt";
	Sudoku sudoku;

	input.analyze();

	/* test the performance
	off = clock();
	cout << "total time:" << (off - on) / CLOCKS_PER_SEC << endl;
	*/
	return 0;
}

