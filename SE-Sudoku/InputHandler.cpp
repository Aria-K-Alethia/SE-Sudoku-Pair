#include "stdafx.h"
#include "InputHandler.h"
#include "regex"
#include "Output.h"
#include "string"
#include "sstream"
#include "ctype.h"
#include "iostream"

#define MAXPUZZLECOUNT

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
	//check the format of the param and save them if valid
	if (argc < 3)Output::error(1);
	string m(argv[1]);
	if (m == "-s" || m == "-c") {
		if (argc != 3) Output::error(1);
		mode = m[1];
	}
	else {
		if (argc > 6) Output::error(1);
		string param[6];
		for (int i = 0; i < argc; ++i) {
			param[i] = argv[i];
			if (param[i] == "-n") {
				mode = param[i][1];
			}
		}
		if (mode != 'n') Output::error(8);
		hardness = 0;
		unique = false;
		upper = 0;
		lower = 0;
		for (int i = 1; i < argc; ++i) {
			if (param[i] == "-r") {
				regex p("^\\d{2}~\\d{2}$");
				smatch m;
				if (regex_match(param[i + 1], m, p)) {
					int pos = param[i + 1].find("~");
					string num1 = param[i + 1].substr(0, pos+1);
					string num2 = param[i + 1].substr(pos + 1);
					stringstream ss;
					ss << num1;
					ss >> lower;
					ss << num2;
					ss >> upper;
					if (lower < 20 || upper > 55) Output::error(9);
					if (lower > upper) Output::error(15); 
				}
				else {
					Output::error(9);
				}
			}
			else if (param[i] == "-m") {
				regex p("^[1-3]$");
				smatch m;
				if (regex_match(param[i + 1], m, p)) {
					stringstream ss;
					ss << param[i + 1];
					ss >> hardness;
				}
				else Output::error(10);
			}
			else if (param[i] == "-u") {
				unique = true;
			}

			else if (param[i] == "-n") {
				string num = param[i + 1];
				regex p("^\\d+$");
				smatch m;
				if (regex_match(num, m, p)) {
					stringstream ss;
					ss << num;
					ss >> number;
					if (number < 1 || number > 10000) Output::error(13);
				}
				else Output::error(14);
			}
		}
		if (hardness != 0 && upper != 0 && lower != 0) Output::error(11);
		if (hardness != 0 && unique) Output::error(11);
	}

	if (mode == 's') {
		filename = argv[2];
		fstream file(filename, ios::in);
		if (file.is_open()) file.close();
		else Output::error(4);
	}
	else if(mode == 'c') {
		regex p("^\\d+$");
		smatch m;
		string num(argv[2]);
		if (regex_match(num, m, p)) {
			stringstream ss;
			ss << num;
			ss >> number;
			if (number < 1 || number > 1000000)Output::error(12);
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

bool InputHandler::get_board(fstream &file, int board[LEN*LEN])
{
	/*
	@overview:get a board from file,if the format is wrong,raise error
	*/
	regex p("^(\\d\\s){8}\\d$");
	smatch m;
	string s;
	//should have 10 lines for a single board;
    int j = 0;
	for (int i = 1; i <= LEN; ++i) {
		if (!getline(file, s)) return false;
		//cout << s.length() << endl;
		if (!regex_match(s, m, p)) {
			file.close();
			Output::error(5);
		}
		for (int k = 0; (unsigned)k < s.length(); ++k) {
			if (!isspace(s[k])) {
				board[j] = s[k];
				++j;
			}
		}
	}
	getline(file, s);
	return true;
}

