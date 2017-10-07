#pragma once
#include "fstream"
#include "regex"
#include "sstream"
using namespace std;
#define LEN 9

/*
@overview:The InputHandler is mutable.This class is used to processing the input and save necessary info.
*/
class InputHandler {
public:
	InputHandler(int argc, char** argv);
	void analyze();
	char getMode();
	char* getFileName();
	int getNumber();
	bool getBoard(fstream &file, int *board);
	int getUpper();
	int getLower();
	bool getUnique();
	int getHardness();

	//private:
	int argc;
	char** argv;
	char mode;
	int hardness;
	int upper;
	int lower;
	bool unique;
	char* filename;
	int number;
};