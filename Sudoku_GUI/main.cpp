#include "Sudoku_GUI.h"
#include <QtWidgets/QApplication>
#include "QFile"



static void setStyle() {
	QFile qss(STYLE_FILE_NAME);
	qss.open(QFile::ReadOnly);
	qApp->setStyleSheet(qss.readAll());
	qss.close();
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Sudoku_GUI w;
	setStyle();
	w.show();
	return a.exec();
}
