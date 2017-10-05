#pragma once
#include "fstream"
using namespace std;
#define LEN 9
/*
@overview:The InputHandler is mutable.This class is used to processing the input and save necessary info.
*/
class InputHandler
{
public:
	InputHandler(int argc, char** argv);
	void analyze();
	char get_mode();
	char* get_filename();
	int get_number();
	bool get_board(fstream &file, char board[][LEN + 1]);

	//private:
	int argc;
	char** argv;
	char mode;
	char* filename;
	int number;
};