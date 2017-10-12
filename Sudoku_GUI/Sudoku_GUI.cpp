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
#include "QTimer"
#include "QTime"
#include "QFile"
#include <iostream>
#include <fstream>
#pragma comment(lib,"SudokuDll.lib")

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 960
#define WIN_GAME 1
#define LOSE_GAME 0
#define NOT_COMPLETE -1
#define STYLE_FILE_NAME "../Sudoku_GUI/Resources/stylesheet"

static QString welcomePage1Str[2] = { "NewGame","help" };
static QString welcomePage2Str[3] = { "Easy","Medium","Hard" };
char* timeRecordFileName = "timerecord.txt";
static int currentX = -1;
static int currentY = -1;
static bool tableClickable[LEN][LEN] = { 0 };
static int level;

Sudoku_GUI::Sudoku_GUI(QWidget *parent)
	: QMainWindow(parent),
	sudoku(nullptr)
{
	ui.setupUi(this);
	this->setWindowTitle(tr("Sudoku Game"));
	sudoku = new Sudoku();
    begin = false;
    initRecord();
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
	QWidget* welcomeWidgetPage3 = new QWidget();
	welcomeWindow->addWidget(welcomeWidgetPage1);
	welcomeWindow->addWidget(welcomeWidgetPage2);
	welcomeWindow->addWidget(welcomeWidgetPage3);
	welcomeWindow->setCurrentIndex(0);

	//welcome window page 1
	QVBoxLayout* welcomePage1Layout = new QVBoxLayout(welcomeWidgetPage1);
	welcomePage1Layout->addStretch(1);
	QLabel* welcomeLabel = new QLabel(tr("Sudoku Game"));
	welcomeLabel->setAlignment(Qt::AlignHCenter);
	welcomePage1Layout->addWidget(welcomeLabel);
	welcomePage1Layout->addStretch(2);
	welcomePage1Layout->setMargin(200);
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

	//welcome window page 3
	QVBoxLayout* welcomePage3Layout = new QVBoxLayout(welcomeWidgetPage3);
	char* helpTitle = "Sudoku Rule";
	char* helpInfo = "Sudoku is a logic-based,combinatorial number-placement puzzle.\n\
The objective is to fill a 9x9 block with digits so that\n each column, each row,and \
each of the nine 3x3 sub-bloc\n that compose the block contains all of the digits\
 from 1 to 9.\n The puzzle setter provides a partially completed grid,\nwhich for a \
well-posed puzzle has a single solution.";
	QLabel* helpInfoTitle = new QLabel(tr(helpTitle));
	QLabel* helpInfoLabel = new QLabel(tr(helpInfo));
	helpInfoTitle->setAlignment(Qt::AlignHCenter);
	helpInfoLabel->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
	welcomePage3Layout->addWidget(helpInfoTitle);
	welcomePage3Layout->addStretch(1);
	welcomePage3Layout->addWidget(helpInfoLabel);
	welcomePage3Layout->addStretch(1);
	QPushButton *button = new QPushButton(QString("Return"));
	connect(button, &QPushButton::clicked, this, &Sudoku_GUI::pressButtonReturn);
	welcomePage3Layout->addWidget(button);
	welcomePage3Layout->setMargin(50);
	welcomePage3Layout->setSpacing(0);

	//Main game page
	QVBoxLayout* mainLayout = new QVBoxLayout(gameWindow);
	QGridLayout* puzzleLayout = new QGridLayout(gameWindow);
	QHBoxLayout* hintAndTimerLayout = new QHBoxLayout(gameWindow);
	QHBoxLayout* choicesLayout = new QHBoxLayout(gameWindow);
    
    // Set spacers for puzzle layout
        // Vertical spacers
    puzzleLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding), 0, 0, 1, LEN + 2);
    puzzleLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding), LEN + 1, 0, 1, LEN + 2);
        // Horizontal spacers
    puzzleLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum), 1, 0, LEN, 1);
    puzzleLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum), 1, LEN + 1, LEN, 1);
	
    // Add puzzle buttons
	puzzleButtons = new QPushButton**[LEN];
	for (int i = 0; i < LEN; i++) {
		puzzleButtons[i] = new QPushButton*[LEN];
        for (int j = 0; j < LEN; j++) {
            puzzleButtons[i][j] = new QPushButton();
            puzzleButtons[i][j]->setMinimumSize(60, 60);
            puzzleButtons[i][j]->setMaximumSize(60, 60);
            // Set button style
            if (i == 0 && j == 0) {
                puzzleButtons[i][j]->setObjectName("puzzleButtonTLCorner");
            } else if (i == 0 && j == LEN - 1) {
                puzzleButtons[i][j]->setObjectName("puzzleButtonTRCorner");
            } else if (i == LEN - 1 && j == 0) {
                puzzleButtons[i][j]->setObjectName("puzzleButtonBLCorner");
            } else if (i == LEN - 1 && j == LEN - 1) {
                puzzleButtons[i][j]->setObjectName("puzzleButtonBRCorner");
            } else {
                puzzleButtons[i][j]->setObjectName("puzzleButton");
            }
			puzzleButtons[i][j]->setAccessibleName(QString::number(i*LEN + j));
			puzzleLayout->addWidget(puzzleButtons[i][j], i + 1, j + 1, 1, 1);
			connect(puzzleButtons[i][j], &QPushButton::clicked, this, &Sudoku_GUI::pressButtonPuzzle);
		}
	}
	//puzzleLayout set
	//puzzleLayout->setMargin(90);
    //puzzleLayout->set
    puzzleLayout->setSpacing(0);

	//hintAndTimer:layout and component
        // Hint
	QPushButton *hintButton = new QPushButton("Hint");
	hintButton->setMinimumSize(QSize(60, 60));
	hintButton->setMaximumSize(QSize(60, 60));
    hintAndTimerLayout->addStretch(0);
	hintAndTimerLayout->addWidget(hintButton);
	connect(hintButton, &QPushButton::clicked, this, &Sudoku_GUI::pressButtonHint);
	
        // Timer
    timer = new QTimer(this);
    timeLabel = new QLabel(this);
    timeRecord = new QTime(0, 0, 0);
    timeLabel->setText(timeRecord->toString("hh:mm:ss"));
    hintAndTimerLayout->addStretch(1);
    hintAndTimerLayout->addWidget(timeLabel);
    connect(timer, &QTimer::timeout, this, &Sudoku_GUI::timeUpdate);

	//Add choices buttons
	QPushButton* choicesButtons[LEN];
	QString temp;
	for (int i = 0; i < LEN; i++) {
		choicesButtons[i] = new QPushButton(temp.setNum(i+1));
		choicesButtons[i]->setMinimumSize(QSize(60, 60));
		choicesButtons[i]->setMaximumSize(QSize(60, 60));
		choicesButtons[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		connect(choicesButtons[i], &QPushButton::clicked, this, &Sudoku_GUI::pressButtonChoice);
		choicesLayout->addWidget(choicesButtons[i], 1);
	}
	choicesLayout->setMargin(0);
	//Add all components to mainLayout
	mainLayout->setSpacing(0);
    mainLayout->addStretch(1);
	mainLayout->addLayout(puzzleLayout, 1);
    mainLayout->addStretch(1);
	mainLayout->addLayout(hintAndTimerLayout, 1);
    mainLayout->addStretch(1);
	mainLayout->addLayout(choicesLayout, 1);

    //Menu start game
    for (int i = 0; i < 3; ++i) {
        QAction *newGameMenuAction = new QAction(welcomePage2Str[i]);
        ui.menuNewGame->addAction(newGameMenuAction);
        connect(newGameMenuAction, &QAction::triggered, this, &Sudoku_GUI::newGameMenuClicked);
    }

	this->setMinimumSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT));
	this->setMaximumSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT));

	//welcomeWindow->setCurrentIndex(1); // Debugging code
}

// Button methods beneath
void Sudoku_GUI::pressButtonWelcome() {
	//overview: Choose to enter game or view help
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if (button->text() == welcomePage1Str[0]) {
		welcomeWindow->setCurrentIndex(1);
	}
	else if (button->text() == welcomePage1Str[1]) {
		welcomeWindow->setCurrentIndex (2);
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
	if (tableClickable[x][y]) { // Numbers in unclickable button can't be changed
        if (currentX != -1 && currentY != -1) { // Unchecked pressed puzzle button 
            puzzleButtons[currentX][currentY]->setChecked(false);
        }
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
		puzzleButtons[currentX][currentY]->setChecked(false); // Set button unchecked
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
    int* board = new int[LEN*LEN];
    int* solution = new int[LEN*LEN];
    for (int i = 0; i < LEN; ++i) {
        for (int j = 0; j < LEN; ++j) {
            QString temp = puzzleButtons[i][j]->text();
            int num;
            if (temp == "" || (i == currentX && j == currentY)) num = 0;
            else num = temp.toInt();
            board[i*LEN + j] = num;
        }
    }
	if (sudoku->solve(board, solution)) {
		puzzleButtons[currentX][currentY]->setText(QString::number(solution[currentX*LEN + currentY]));
		puzzleButtons[currentX][currentY]->setChecked(false); // Set button unchecked
		checkGame();
	}
	else {
		QMessageBox::information(this, tr("Bad Sudoku"), tr("Can not give a hint.The current Sudoku\
 is not valid\nPlease check the row,rolumn or 9x9 block to correct it."));
	}
	
}

void Sudoku_GUI::pressButtonReturn()
{
	welcomeWindow->setCurrentIndex(0);
}

void Sudoku_GUI::gameSet(int degOfDifficulty) {
	/*
		@overview:invoked when player choose the degree of difficulty,init the game.
	*/
	
    resetTimer();
    level = degOfDifficulty;
    begin = true;
	int result[10][LEN*LEN];
	sudoku->generate(10, degOfDifficulty, result);
	srand((unsigned)time(nullptr));
	int target = rand() % 10;
	QString temp;
	QString vac("");
	for (int i = 0; i < LEN; ++i) {
		for (int j = 0; j < LEN; ++j) {
			if (result[target][i*LEN + j] == 0) {
				tableClickable[i][j] = true;
				puzzleButtons[i][j]->setText(vac);
				puzzleButtons[i][j]->setEnabled(true);
				puzzleButtons[i][j]->setCheckable(true); // Able to be checked
			}
			else {
				tableClickable[i][j] = false;
				puzzleButtons[i][j]->setText(temp.setNum(result[target][i*LEN + j]));
				puzzleButtons[i][j]->setEnabled(false); // Unable to be editted
			}
		}
	}
    //start timer
    timer->start(1000);
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
		@overview:check whether the game is win,lose,or not complete,return the corresponding signal
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
    delete board;
	int flag;
	if (sudoku->check()) {
		flag =  WIN_GAME;
	}
	else flag = LOSE_GAME;
	gameCompleted(flag);
	return flag;
}

void Sudoku_GUI::gameCompleted(int flag) {
    /*
    		@overview:invoked when the game board is full of number,check the game is valid or not,
    		showing the corresponding message.
    */
	if (flag == LOSE_GAME) {
        timer->stop();
        if (QMessageBox::Ok == QMessageBox::information(this, QObject::tr("You Lose"), QObject::tr\
             ("The currently game you have solved is not valid,try to find the error and fix it"))) {
            timer->start();
        }
    }
	else if(flag == WIN_GAME) {
        timer->stop();
        checkTimeRecord();
		begin = false;
        if (QMessageBox::Yes == QMessageBox::question(NULL, QObject::tr("You Win"), QObject::tr\
             ("Congradulations!\nYou have won this game!\nStart a new game?"), \
             QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)) {
            welcomeWindow->setCurrentIndex(1);
            mainWindow->setCurrentIndex(0);
            
        } else {
            welcomeWindow->setCurrentIndex(0);
            mainWindow->setCurrentIndex(0);
        }
	}
}

void Sudoku_GUI::timeUpdate() {
    /*
    @overview:update timerecord every sec and update the time on timeLabel
    */
    *timeRecord = timeRecord->addSecs(1);
    QString timeString = timeRecord->toString("hh:mm:ss");
    timeLabel->setText(timeString);
}

void Sudoku_GUI::newGameMenuClicked() {
    /*
    @overview:triggered when the new game action clicked on the menu
    */
    if (begin) {
        timer->stop();
        if (QMessageBox::No == QMessageBox::question(this, tr("NewGame"), \
            tr("Are you sure to discard the current game?"), \
            QMessageBox::Yes | QMessageBox::No, \
            QMessageBox::Yes)) {
            timer->start(1000);
            return;
        }
    }
    QAction *action = qobject_cast<QAction*>(sender());
    int i = 0;
	for (; i < 3; ++i) {
		if (action->text() == welcomePage2Str[i]) break;
	}
    gameSet(i + 1);
	mainWindow->setCurrentIndex(1);
}

void Sudoku_GUI::resetTimer() {
    /*
    @overview:reset the timer and timerecord for the next game
    */
    timer->stop();
    timeRecord->setHMS(0, 0, 0);
    QString timeString = timeRecord->toString("hh:mm:ss");
    timeLabel->setText(timeString);
}

void Sudoku_GUI::checkTimeRecord() {
    /*
    @overview:invoked when player solves a game,check the time record in file,showing
    corresponding message,save new time record if exists.
    */
    fstream timeRecordFile(timeRecordFileName, ios::in);
    if (!timeRecordFile.is_open()) {
        timeRecordFile.close();
        initRecord();
    } else timeRecordFile.close();

    timeRecordFile.open(timeRecordFileName, ios::in);
    string record[3];
    for (int i = 0; i < 3; ++i) {
        getline(timeRecordFile, record[i]);
    }
    timeRecordFile.close();
    QTime old;
    int oldtime[3];
    int count = 0;
    int pos = 0;
    char buffer[50] = { 0 };
    for (int i = 0; i < record[level].length(); ++i) {
        if (record[level][i] == ':') {
            buffer[pos] = '\0';
            oldtime[count] = atoi(buffer);
            count++;
            pos = 0;
        } else {
            buffer[pos] = record[level][i];
            pos++;
        }
    }
    old.setHMS(oldtime[0], oldtime[1], oldtime[2]);
    if (old > *timeRecord) {
        QMessageBox::information(this, tr("Record"), "You have not break the record in this mode\n\
the current record in this mode is:" + old.toString("hh::mm::ss"));

    } else {
        QMessageBox::information(this, tr("Record"), "Congradulation!You have broken the record in\
in this mode\nThe new record is:" + timeRecord->toString("hh:mm:ss"));
        timeRecordFile.open(timeRecordFileName, ios::out);
        for (int i = 0; i < 3; ++i) {
            if (i != level)
                timeRecordFile << record[level] << endl;
            else
                timeRecordFile << timeRecord->toString("hh:mm:ss").toStdString() << endl;
        }
        timeRecordFile.close();
    }

}

void Sudoku_GUI::initRecord() {
    /*
    @overview:init the record file
    */
    fstream timeRecordFile(timeRecordFileName, ios::in);
    if (!timeRecordFile.is_open()) {
        //not exist,construct one
        timeRecordFile.close();
        timeRecordFile.open(timeRecordFileName, ios::out);
        char* header = "0:0:0\n0:0:0\n0:0:0\n";
        timeRecordFile << header;
    }
    timeRecordFile.close();
}


void Sudoku_GUI::setStyle() {
	QFile qss(STYLE_FILE_NAME);
	qss.open(QFile::ReadOnly);
	qApp->setStyleSheet(qss.readAll());
	qss.close();
}