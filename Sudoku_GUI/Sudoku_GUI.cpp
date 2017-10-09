#include "Sudoku_GUI.h"
#include "ui_Sudoku_GUI.h"
#include "QStackedWidget"
#include "QVBoxLayout"
#include "QPushButton"
#include "QLabel"
#include "QGridLayout"
#include "QHBoxLayout"
#include "Sudoku.h"
#include "QMessageBox"
#include <iostream>
#pragma comment(lib,"SudokuDll.lib")

#define WINDOW_SIZE 1000
#define WIN_GAME 1
#define LOSE_GAME 0
#define NOT_COMPLETE -1

static QString welcomePage1Str[2] = { "NewGame","help" };
static QString welcomePage2Str[3] = { "Easy","Medium","Hard" };
static int currentX = -1;
static int currentY = -1;
static bool tableClickable[LEN][LEN] = { 0 };
static int solution[LEN*LEN];

Sudoku_GUI::Sudoku_GUI(QWidget *parent)
	: QMainWindow(parent),
	sudoku(nullptr)
{
	ui.setupUi(this);
	this->setWindowTitle(tr("Sudoku Game"));
	sudoku = new Sudoku();
	//widget
	mainWindow = new QStackedWidget();
	welcomeWindow = new QStackedWidget();
	gameWindow = new QWidget();
	mainWindow->addWidget(welcomeWindow);
	mainWindow->addWidget(gameWindow);
	mainWindow->setCurrentIndex(0);
	this->setCentralWidget(mainWindow);

	//welcome window
	QWidget* welcomeWidgetPage1 = new QWidget();
	QWidget* welcomeWidgetPage2 = new QWidget();
	welcomeWindow->addWidget(welcomeWidgetPage1);
	welcomeWindow->addWidget(welcomeWidgetPage2);
	welcomeWindow->setCurrentIndex(0);

	//welcome window page 1
	QVBoxLayout* welcomePage1Layout = new QVBoxLayout(welcomeWidgetPage1);
	welcomePage1Layout->addStretch(1);
	QLabel* welcomeLabel = new QLabel(tr("Sudoku Game"));
	welcomeLabel->setAlignment(Qt::AlignHCenter);
	welcomePage1Layout->addWidget(welcomeLabel);
	welcomePage1Layout->addStretch(2);
	welcomePage1Layout->setMargin(180);
	for (int i = 0; i < 2; ++i) {
		QPushButton *button = new QPushButton(welcomePage1Str[i]);
		welcomePage1Layout->addWidget(button);
		connect(button, &QPushButton::clicked, this, &Sudoku_GUI::pressButtonWelcome);
		welcomePage1Layout->addStretch(1);
	}


	//welcome window page 2
	QVBoxLayout* welcomePage2Layout = new QVBoxLayout(welcomeWidgetPage2);
	QLabel* chooseDifficultyLabel = new QLabel(tr("choose difficulty"));
	chooseDifficultyLabel->setAlignment(Qt::AlignHCenter);
	welcomePage2Layout->addWidget(chooseDifficultyLabel);
	welcomePage2Layout->addStretch(2);
	welcomePage2Layout->setMargin(180);
	for (int i = 0; i < 3; ++i) {
		QPushButton *button = new QPushButton(welcomePage2Str[i]);
		connect(button, &QPushButton::clicked, this, &Sudoku_GUI::pressButtonDifficulty);
		welcomePage2Layout->addWidget(button);
		welcomePage2Layout->addStretch(1);
	}

	//Main game page
	QVBoxLayout* mainLayout = new QVBoxLayout(gameWindow);
	QGridLayout* puzzleLayout = new QGridLayout(gameWindow);
	QHBoxLayout* hintAndTimerLayout = new QHBoxLayout(gameWindow);
	QHBoxLayout* choicesLayout = new QHBoxLayout(gameWindow);
	//Add puzzle buttons
	puzzleButtons = new QPushButton**[LEN];
	for (int i = 0; i < LEN; i++) {
		puzzleButtons[i] = new QPushButton*[LEN];
		for (int j = 0; j < LEN; j++) {
			puzzleButtons[i][j] = new QPushButton();
			puzzleButtons[i][j]->setMinimumSize(80,80);
			puzzleButtons[i][j]->setStyleSheet("QPushButton{border:1px;background-color:white;border-style:solid}");
			puzzleButtons[i][j]->setMaximumSize(80, 80);
			puzzleButtons[i][j]->setAccessibleName(QString::number(i*LEN + j));
			puzzleLayout->addWidget(puzzleButtons[i][j], i, j);
			connect(puzzleButtons[i][j], &QPushButton::clicked, this, &Sudoku_GUI::pressButtonPuzzle);
			puzzleButtons[i][j]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		}
	}
	//puzzleLayout set
	puzzleLayout->setMargin(140);
	puzzleLayout->setVerticalSpacing(0);
	puzzleLayout->setHorizontalSpacing(0);

	/*for (int i = 0; i < 9; ++i) {
		puzzleLayout->setColumnStretch(i, 1);
		puzzleLayout->setRowStretch(i, 1);
		puzzleLayout->setColumnMinimumWidth(i, 20);
	}*/

	//hintAndTimer:layout and component
	QPushButton *button = new QPushButton("Hint");
	button->setMinimumSize(QSize(80, 40));
	button->setMaximumSize(QSize(80, 40));
	hintAndTimerLayout->addWidget(button);
	connect(button, &QPushButton::clicked, this, &Sudoku_GUI::pressButtonHint);
	

	//Add choices buttons
	QPushButton* choicesButtons[9];
	QString temp;
	for (int i = 0; i < 9; i++) {
		choicesButtons[i] = new QPushButton(temp.setNum(i+1));
		choicesButtons[i]->setMinimumSize(QSize(80, 80));
		choicesButtons[i]->setMaximumSize(QSize(80, 80));
		choicesButtons[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		connect(choicesButtons[i], &QPushButton::clicked, this, &Sudoku_GUI::pressButtonChoice);
		choicesLayout->addWidget(choicesButtons[i], 1);

	}
	choicesLayout->setMargin(0);
	//Add all components to mainLayout
	mainLayout->setSpacing(0);
	mainLayout->addLayout(puzzleLayout, 1);
	mainLayout->addLayout(hintAndTimerLayout, 1);
	mainLayout->addLayout(choicesLayout, 1);

	this->setMinimumSize(QSize(WINDOW_SIZE, WINDOW_SIZE));
	this->setMaximumSize(QSize(WINDOW_SIZE, WINDOW_SIZE));

}

void Sudoku_GUI::pressButtonWelcome() {
	//overview: Choose to enter game or view help
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if (button->text() == welcomePage1Str[0]) {
		welcomeWindow->setCurrentIndex(1);
	}
	else if (button->text() == welcomePage1Str[1]) {
		//welcomeWindow->setCurrentIndex (2);
	}
}


void Sudoku_GUI::pressButtonDifficulty() {
	//overview: Switch to game view when difficulty button pressed
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	int i;
	for (i = 0; i < 3; ++i) {
		if (button->text() == welcomePage2Str[i]) {
			break;
		}
	}
	mainWindow->setCurrentIndex(1);
	//Generate sudoku puzzle with different difficulty
	gameSet(i + 1);
}

void Sudoku_GUI::pressButtonPuzzle() {
	/*
		@overview: Invoked when puzzle buttons pressed
		check the position if it is clickable,if so,set current x and y;
	*/
	QPushButton *button = qobject_cast<QPushButton*>(sender());

	int name = button->accessibleName().toInt();
	int x = name / LEN;
	int y = name % LEN;
	if (tableClickable[x][y]) {
		currentPositionSet(x, y);
	}
	else {
		currentPositionSet(-1, -1);
	}
}

void Sudoku_GUI::pressButtonChoice() {
	/*
		@overview:invoked if choice button clicked
	*/
	if (currentX == -1 && currentY == -1) {
		return;
	}
	else {
		QPushButton *button = qobject_cast<QPushButton*>(sender());
		int name = button->text().toInt();
		puzzleButtons[currentX][currentY]->setText(QString::number(name));
		checkGame();
		currentPositionSet(-1, -1);
	}
}

void Sudoku_GUI::pressButtonHint()
{
	/*
		@overview:invoked when hint button clicked,give one possible hint on
		currently selected position.
	*/
	if (currentX == -1 || currentY == -1) {
		return;
	}
	puzzleButtons[currentX][currentY]->setText(QString::number(solution[currentX*LEN+currentY]));
	
}

void Sudoku_GUI::gameSet(int degOfDifficulty) {
	/*
		@overview:invoked when player choose the degree of difficulty,set the game board.
	*/
	
	int result[10][LEN*LEN];
	sudoku->generate(10, degOfDifficulty, result);
	srand((unsigned)time(nullptr));
	int target = rand() % 10;
	sudoku->solve(result[target], solution);
	QString temp;
	QString vac("");
	for (int i = 0; i < LEN; ++i) {
		for (int j = 0; j < LEN; ++j) {
			if (result[target][i*LEN + j] == 0) {
				tableClickable[i][j] = true;
				puzzleButtons[i][j]->setText(vac);
			}
			else {
				tableClickable[i][j] = false;
				puzzleButtons[i][j]->setText(temp.setNum(result[target][i*LEN + j]));
			}
		}
	}
}

void Sudoku_GUI::currentPositionSet(int x, int y)
{
	/*
		@overview:set the current postion of game board
	*/
	currentX = x;
	currentY = y;
}

int Sudoku_GUI::checkGame() {
	/*
		@check whether the game is win,lose,or not complete,return the corresponding signal
	*/
	int* board = new int[LEN*LEN];
	for (int i = 0; i < LEN; ++i) {
		for (int j = 0; j < LEN; ++j) {
			QString temp = puzzleButtons[i][j]->text();
			if (temp == "") return NOT_COMPLETE;
			int num = temp.toInt();
			board[i*LEN + j] = num;
		}
	}
	sudoku->convertToTwoDimension(board);
	int flag;
	if (sudoku->check()) {
		flag =  WIN_GAME;
	}
	else flag = LOSE_GAME;
	gameCompleted(flag);
	return flag;
}

void Sudoku_GUI::gameCompleted(int flag) {
	if (flag == LOSE_GAME) {
		QMessageBox::information(NULL, QObject::tr("You Lose"), QObject::tr\
			("The currently game you have solved is not valid,try to find the error and fix it"));
	}
	else if(flag == WIN_GAME) {
		QMessageBox::information(NULL,QObject::tr("You Win"),QObject::tr\
		("Congradulations!\nYou have won this game!"));
	}
}
