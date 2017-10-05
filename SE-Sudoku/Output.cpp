#include "stdafx.h"
#include "iostream"
#include "Output.h"
#include "time.h"

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
	else if (code == 8) cout << "Error code 8:the parameter must contain one of -c,-s,-n" << endl;
	else if (code == 9) cout << "Error code 9:the range of -r must in [20,55]" << endl;
	else if (code == 10)cout << "Error code 10:the range of -m must be 1,2 or 3" << endl;
	else if (code == 11)cout << "Error code 11:-u or -r can not be used with -m" << endl;
	else if (code == 12) cout << "Error code 12:the number of -c must in [1,1000000] " << endl;
	else if (code == 13) cout << "Error code 13:the number of -n must in [1,10000]" << endl;
	else if (code == 14) cout << "Error code 14:the parameter of -n must be a integer" << endl;
	else if (code == 15) cout << "Error code 15:parameter in -r lower~upper must satisfy lower<=upper" << endl;
	exit(-1);
}
