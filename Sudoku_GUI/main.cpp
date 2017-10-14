#include "Sudoku_GUI.h"
#include <QtWidgets/QApplication>
#include "QFile"
#include <iostream>
#include <fstream>

static void setStyle() {
	QFile qss(STYLE_FILE_NAME);
	qss.open(QFile::ReadOnly);
	qApp->setStyleSheet(qss.readAll());
	qss.close();
}

static void initializeSaveData() {
    std::fstream file;
    file.open("savedata.txt", std::ios::out);
    int sudokuCount = 3;
    int sudokuLength = 9;
    for (int i = 0; i < sudokuCount; i++) {
        for (int j = 0; j < sudokuLength; j++) {
            for (int k = 0; k < sudokuLength - 1; k++) {
                file << "0 ";
            }
            file << "0\n";
        }
        file << "\n";
    }

    file.close();
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    initializeSaveData();
	Sudoku_GUI w;
	setStyle();
	w.show();
	return a.exec();
}
