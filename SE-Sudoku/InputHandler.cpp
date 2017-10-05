#include "stdafx.h"
#include "InputHandler.h"
#include "regex"
#include "Output.h"
#include "string"
#include "sstream"
#include "ctype.h"
#include "iostream"

using namespace std;
#define LEN 9
/*
@overview:this file implement the class InputHandler in InputHandler.h
*/

//Constructor
InputHandler::InputHandler(int argc, char** argv)
{
	/*
	@overview:init inputhandler
	*/
	this->argc = argc;
	this->argv = argv;
}

void InputHandler::analyze()
{
	/*
	@overview:analyze the param in argv to get attr of inputhandler.
	*/
	//check the number of param
	if (argc != 3) Output::error(1);
	//check the format of the param and save them if valid
	string m(argv[1]);
	if (m == "-s") mode = 's';
	else if (m == "-c") mode = 'c';
	else Output::error(2);

	if (mode == 's')  filename = argv[2];
	else {
		regex p("^\\d+$");
		smatch m;
		string num(argv[2]);
		if (regex_match(num, m, p)) {
			stringstream ss;
			ss << num;
			ss >> number;
		}
		else Output::error(3);
	}
}
char InputHandler::get_mode()
{
	/*
	@overview:getter of mode
	*/

	return mode;
}
char* InputHandler::get_filename()
{
	/*
	@overview:getter of filename
	*/

	return filename;
}
int InputHandler::get_number()
{
	/*
	@overview:getter of number
	*/

	return number;
}

bool InputHandler::get_board(fstream &file, char board[][LEN + 1])
{
	/*
	@overview:get a board from file,if the format is wrong,raise error
	*/
	regex p("^(\\d\\s){8}\\d$");
	smatch m;
	string s;
	//should have 10 lines for a single board;
	for (int i = 1; i <= LEN; ++i) {
		if (!getline(file, s)) return false;
		//cout << s.length() << endl;
		if (!regex_match(s, m, p)) {
			file.close();
			Output::error(5);
		}
		int j = 1;
		for (int k = 0; (unsigned)k < s.length(); ++k) {
			if (!isspace(s[k])) {
				board[i][j] = s[k];
				++j;
			}
		}
	}
	getline(file, s);
	return true;
}