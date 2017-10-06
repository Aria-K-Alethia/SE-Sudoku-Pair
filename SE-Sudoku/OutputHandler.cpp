#include "stdafx.h"
#include "OutputHandler.h"
#include "Output.h"


OutputHandler::OutputHandler(Sudoku* sudoku) {
    this->sudoku = sudoku;
}


void OutputHandler::outputSudoku(int count, int result[][LEN*LEN], char* filename) {
    //overview: output sudoku into designated file
    fstream file(filename, ios::out);
    if (!file.is_open()) Output::error(4);
    for (int i = 0; i < count; i++) {
        this->sudoku->convertToTwoDimension(result[i]);
        char* outcome = this->sudoku->toString();
        file << outcome;
        delete outcome;
    }
    file.close();
}

void OutputHandler::outputSudoku(char* outFileName) {
	/*
		@overview:output one sudoku in its board to outFile with outFile name
	*/
	fstream outFile(outFileName, ios::out);
	outFile << sudoku->toString();
	outFile.close();
}

OutputHandler::~OutputHandler() {
}
