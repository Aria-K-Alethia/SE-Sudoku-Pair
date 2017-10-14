#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Sudoku_GUI.h"

#define STYLE_FILE_NAME "Resources\\stylesheet.qss"
#define LEN 9

class QStackedWidget;
class QWidget;
class QPushButton;
class QTime;
class QTimer;
class QLabel;

class Sudoku_GUI : public QMainWindow
{
	Q_OBJECT

public:
	Sudoku_GUI(QWidget *parent = Q_NULLPTR);

	void setStyle();


private:
	Ui::Sudoku_GUIClass ui;
	QStackedWidget* mainWindow;
	QStackedWidget* welcomeWindow;
	QWidget* gameWindow;
	QPushButton*** puzzleButtons;
    QTimer* timer;
    QTime* timeRecord;
    QLabel* timeLabel;
    bool begin;

	void gameSet(int degOfDiffculty);
    void gameSet(QTime* time, int puzzle[LEN * LEN], bool clickableRecord[LEN * LEN]);
	void currentPositionSet(int x, int y);
	int checkGame();
	void gameCompleted(int flag);
    void timeUpdate();
    void newGameMenuClicked();
    void resetTimer();
    void checkTimeRecord();
    void initRecord();
    void setBackgroundColorForWindow(QWidget* window, int red, int green, int blue);
	void checkWrongAndShow();
	void setWrongColor(int i, int j);
	void setRightColor(int i, int j);
	void saveDataAtIndex(int index);
	void loadDataAtIndex(int index);
    void initSaveData();
    void closeEvent(QCloseEvent *event);
    void saveCurrentGame();
    bool loadAndSetGame();

	void pressButtonWelcome();
	void pressButtonDifficulty();
	void pressButtonPuzzle();
	void pressButtonChoice();
	void pressButtonHint();
	void pressButtonReturn();
    void pressButtonDisplace();
	void pressMenuButtonLoad();
	void pressMenuButtonSave();
};
