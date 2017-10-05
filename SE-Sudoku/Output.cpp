#include "stdafx.h"
#include "iostream"
#include "Output.h"

using namespace std;
/*
@overview:this file implement the class Output in Output.h
*/

Output::Output()
{
}

void Output::error(int code)
{
	/*
	@overview:showing error info on screen corresponding to the code.
	*/

	if (code == 1) cout << "Error code 1:bad number of parameters." << endl;
	else if (code == 2) cout << "Error code 2:bad instruction.expect -c or -s" << endl;
	else if (code == 3) cout << "Error code 3:bad number of instruction -c" << endl;
	else if (code == 4) cout << "Error code 4:bad file name" << endl;
	else if (code == 5) cout << "Error code 5:bad file format" << endl;
	else if (code == 6) cout << "Error code 6:bad file.can not solve the sudoku" << endl;
	else if (code == 7)   cout << "Error code 7:bad generating of sudoku" << endl;
	exit(-1);
}
