#include "stdafx.h"
#include "Output.h"

using namespace std;
/*
@overview:this file implement the class Output in Output.h
*/

Output::Output() {
}

void Output::error(int code) {
    /*
	@overview:showing error info on screen corresponding to the code.
	*/
	if (code == 1) cout << "Error code 1:bad number of parameters." << endl;
	else if (code == 2) cout << "Error code 2:bad instruction.expect -c or -s or -n" << endl;
	else if (code == 3) cout << "Error code 3:bad number of instruction -c" << endl;
	else if (code == 4) cout << "Error code 4:bad file name" << endl;
	else if (code == 5) cout << "Error code 5:bad file format" << endl;
	else if (code == 6) cout << "Error code 6:bad file.can not solve the sudoku" << endl;
	else if (code == 7) cout << "Error code 7:the length of puzzle or solution in solve(puzzle,solution) must be 81" << endl;
	else if (code == 8) cout << "Error code 8:number in generate(int number,int lower,int upper,bool unique,int result[][]) must in[1,10000]" << endl;
	else if (code == 9) cout << "Error code 9:the range of -r must in [20,55]" << endl;
	else if (code == 10)cout << "Error code 10:the range of -m must be 1,2 or 3" << endl;
	else if (code == 11)cout << "Error code 11:-u or -r can not be used with -m" << endl;
	else if (code == 12) cout << "Error code 12:the number of -c must in [1,1000000] " << endl;
	else if (code == 13) cout << "Error code 13:the number of -n must in [1,10000]" << endl;
	else if (code == 14) cout << "Error code 14:the parameter of -n must be a integer" << endl;
	else if (code == 15) cout << "Error code 15:parameter in -r lower~upper must satisfy lower<=upper" << endl;
	cout << "Enter \"sudoku.exe -h\" to get help information" << endl;
	exit(-1);
}

void Output::help()
{
	/*
	@overview:showing error info on screen corresponding to the code.
	*/
	char* help_info = "Usage:\n\
-c number\ngenerate number complete sudoku in sudoku.txt,note number should in [1,1000000]\n\n\
-s filepath\nsolve sudoku puzzle in file with filepath and save the solutions in sudoku.txt\n\n\
-n number [-r lower~upper] [-u]\ngenerate number sudoku puzzle in sudoku.txt,with -r if you\
want to control the holes in puzzle between lower to upper,with -u if you want the puzzles\
have only one solution\nnote valid number should in [1,10000],upper and lower should in\
[20,55]\nNote -n can not be used alone without -r or -u\n\
-n number [-m degree_of_difficulty]\ngenerate number sudoku puzzle in sudoku.txt,with -m \
to control the difficulty of puzzle:easy:1,medium:2,hard:3\n\n";

	cout << help_info;
}
