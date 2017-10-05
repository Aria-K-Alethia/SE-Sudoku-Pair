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
	InputHandler input(argc,argv);
	input.analyze();
	return 0;
}

