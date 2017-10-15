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
#include "QTextStream"
#include <iostream>
#include <fstream>
#pragma comment(lib,"Core.lib")

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 960
#define WIN_GAME 1
#define LOSE_GAME 0
#define NOT_COMPLETE -1
#define STYLE_FILE_NAME "Resources/stylesheet.qss"
#define SAVEDATA_FILE_NAME "savedata.txt"
#define SAVE_CHOICE_COUNT 3

static QString welcomePage1Str[2] = { "NewGame","Help" };
static QString welcomePage2Str[3] = { "Easy","Medium","Hard" };
static QString saveChoices[SAVE_CHOICE_COUNT] = { "Save record 1", "Save record 2", "Save record 3" };
static QString loadChoices[SAVE_CHOICE_COUNT] = { "Load record 1", "Load record 2", "Load record 3" };
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
    setBackgroundColorForWindow(gameWindow, 40, 40, 40);
	mainWindow->addWidget(welcomeWindow);
	mainWindow->addWidget(gameWindow);
	mainWindow->setCurrentIndex(0);
	this->setCentralWidget(mainWindow);

	//welcome window
	QWidget* welcomeWidgetPage1 = new QWidget();
	QWidget* welcomeWidgetPage2 = new QWidget();
	QWidget* welcomeWidgetPage3 = new QWidget();
    setBackgroundColorForWindow(welcomeWidgetPage1, 40, 40, 40);
    setBackgroundColorForWindow(welcomeWidgetPage2, 40, 40, 40);
    setBackgroundColorForWindow(welcomeWidgetPage3, 40, 40, 40);
	welcomeWindow->addWidget(welcomeWidgetPage1);
	welcomeWindow->addWidget(welcomeWidgetPage2);
	welcomeWindow->addWidget(welcomeWidgetPage3);
	welcomeWindow->setCurrentIndex(0);

	//welcome window page 1
    QVBoxLayout* welcomePage1Layout = new QVBoxLayout(welcomeWidgetPage1);
	welcomePage1Layout->addStretch(1);
	QLabel* welcomeLabel = new QLabel(tr("Sudoku Game"));
    welcomeLabel->setObjectName("subtitleLabel");
	welcomeLabel->setAlignment(Qt::AlignHCenter);
	welcomePage1Layout->addWidget(welcomeLabel);
	welcomePage1Layout->addStretch(2);
	welcomePage1Layout->setMargin(200);
        //Add new game & help buttons
	for (int i = 0; i < 2; ++i) {
		QPushButton *button = new QPushButton(welcomePage1Str[i]);
		welcomePage1Layout->addWidget(button);
		connect(button, &QPushButton::clicked, this, &Sudoku_GUI::pressButtonWelcome);
		welcomePage1Layout->addStretch(1);
	}


	//welcome window page 2
	QVBoxLayout* welcomePage2Layout = new QVBoxLayout(welcomeWidgetPage2);
	QLabel* chooseDifficultyLabel = new QLabel(tr("Choose Difficulty"));
    chooseDifficultyLabel->setObjectName("subtitleLabel");
	chooseDifficultyLabel->setAlignment(Qt::AlignHCenter);
	welcomePage2Layout->addWidget(chooseDifficultyLabel);
	welcomePage2Layout->addStretch(2);
	welcomePage2Layout->setMargin(180);
        // Add difficulty choice buttons
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
    helpInfoTitle->setObjectName("subtitleLabel");
    helpInfoLabel->setObjectName("highlightLabel");
	welcomePage3Layout->addWidget(helpInfoTitle, 1, Qt::AlignHCenter);
	welcomePage3Layout->addWidget(helpInfoLabel, 1, Qt::AlignHCenter | Qt::AlignTop);
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
                puzzleButtons[i][j]->setObjectName("puzzleButtonTLCorner"); // Top left
            } else if (i == 0 && j == LEN - 1) {
                puzzleButtons[i][j]->setObjectName("puzzleButtonTRCorner"); // Top right
            } else if (i == LEN - 1 && j == 0) {
                puzzleButtons[i][j]->setObjectName("puzzleButtonBLCorner"); // Bottom left
            } else if (i == LEN - 1 && j == LEN - 1) {
                puzzleButtons[i][j]->setObjectName("puzzleButtonBRCorner"); // Bottom right
            } else if (j == 2 || j == 5) {
                if (i == 2 || i == 5) {
                    puzzleButtons[i][j]->setObjectName("puzzleButtonBRE"); //Bottom Right edge
                } else {
                    puzzleButtons[i][j]->setObjectName("puzzleButtonRE"); // Right edge
                }
            } else if (i == 2 || i == 5) {
                puzzleButtons[i][j]->setObjectName("puzzleButtonBE"); // Bottom edge
            } else {
                puzzleButtons[i][j]->setObjectName("puzzleButton");
            }
			puzzleButtons[i][j]->setAccessibleName(QString::number(i*LEN + j));
			puzzleLayout->addWidget(puzzleButtons[i][j], i + 1, j + 1, 1, 1);
			connect(puzzleButtons[i][j], &QPushButton::clicked, this, &Sudoku_GUI::pressButtonPuzzle);
		}
	}
    puzzleLayout->setSpacing(0);

	//Hint, Timer and Displace:layout and component
        // Hint
	QPushButton *hintButton = new QPushButton("Hint");
    hintButton->setObjectName("blueButton");
	hintAndTimerLayout->addWidget(hintButton, 1, Qt::AlignHCenter);
	connect(hintButton, &QPushButton::clicked, this, &Sudoku_GUI::pressButtonHint);
	
        // Timer
    timer = new QTimer(this);
    timeLabel = new QLabel(this);
    timeLabel->setObjectName("highlightLabel");
    timeRecord = new QTime(0, 0, 0);
    timeLabel->setText(timeRecord->toString("hh:mm:ss"));
    hintAndTimerLayout->addWidget(timeLabel, 1, Qt::AlignHCenter);
    connect(timer, &QTimer::timeout, this, &Sudoku_GUI::timeUpdate);

        //Displace
    QPushButton *displaceButton = new QPushButton("Displace");
    displaceButton->setObjectName("displaceButton");
    hintAndTimerLayout->addWidget(displaceButton, 1, Qt::AlignHCenter);
    connect(displaceButton, &QPushButton::clicked, this, &Sudoku_GUI::pressButtonDisplace);

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

    // Menu start game
    for (int i = 0; i < 3; ++i) {
        QAction *newGameMenuAction = new QAction(welcomePage2Str[i]);
        ui.menuNewGame->addAction(newGameMenuAction);
        connect(newGameMenuAction, &QAction::triggered, this, &Sudoku_GUI::pressMenuButtonNewGame);
    }

    // Menu save & load game
    for (int i = 0; i < SAVE_CHOICE_COUNT; i++) {
        QAction *saveMenuAction = new QAction(saveChoices[i]);
        QAction *loadMenuAction = new QAction(loadChoices[i]);
        ui.menuSave->addAction(saveMenuAction);
        connect(saveMenuAction, &QAction::triggered, this, &Sudoku_GUI::pressMenuButtonSave);
        ui.menuLoad->addAction(loadMenuAction);
        connect(loadMenuAction, &QAction::triggered, this, &Sudoku_GUI::pressMenuButtonLoad);
    }

    // Window size set fixxed 
	this->setMinimumSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT));
	this->setMaximumSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT));

	//welcomeWindow->setCurrentIndex(1); // Debugging code
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

void Sudoku_GUI::setBackgroundColorForWindow(QWidget* window, int red, int green, int blue) {
    QPalette pal(window->palette());
    QColor color = QColor(red, green, blue);
    pal.setColor(QPalette::Background, color);
    window->setAutoFillBackground(true);
    window->setPalette(pal);
    window->show();
}

void Sudoku_GUI::saveDataAtIndex(int index) {
    // Read all contents from file
    QFile readFile(SAVEDATA_FILE_NAME);
    QString allContents;
    if (readFile.open(QIODevice::Text | QIODevice::ReadOnly)) {
        QTextStream readStream(&readFile);
        allContents = readStream.readAll();
        readFile.close();
    }

    //Change contents and write back to file
    QFile writeFile(SAVEDATA_FILE_NAME);
    if (writeFile.open(QIODevice::Text | QIODevice::WriteOnly)) {
        QTextStream writeStream(&writeFile);
        QStringList contentsList = allContents.split("\n");
        int startIndex = index * (LEN + 1);
        for (int i = 0; i < LEN; i++) {
            QString* tempString = &(contentsList[startIndex + i]);
            for (int j = 0, k = 0; j < contentsList[startIndex + i].length(); j++) {
                if (tempString->at(j) == ' ') { // Ignore spaces
                    continue;
                } else {
                    QString newContent = puzzleButtons[i][k++]->text();
                    if (newContent == "") {
                        tempString->replace(j, 1, "0");
                    } else {
                        tempString->replace(j, 1, newContent);
                    }
                }
            }
        }

        // Write back to file
        for (int i = 0; i < contentsList.size(); i++) {
            writeStream << contentsList[i] << "\n";
        }

        writeFile.close();
    }
}

void Sudoku_GUI::loadDataAtIndex(int index) {
    QFile dataFile(SAVEDATA_FILE_NAME);
    if (dataFile.open(QIODevice::ReadOnly)) {
        QTextStream fileStream(&dataFile);
        
        // Skip all uneccessary lines
        for (int i = 0; i < index; i++) {
            for (int j = 0; j < LEN + 1; j++) { // Skip a sudoku + one extra line
                fileStream.readLine();
            }
        }

        // Load a sudoku
        for (int i = 0; i < LEN; i++) {
            QString line = fileStream.readLine();
            line.remove(" ");

            // Load one line into game board
            for (int j = 0; j < LEN; j++) {
                if (line[j] == '0') {
                    puzzleButtons[i][j]->setText(QString(""));
                } else {
                    puzzleButtons[i][j]->setText(QString(line[j]));
                }
            }
        }
        dataFile.close();
    }
}

// Button methods beneath
void Sudoku_GUI::pressButtonWelcome() {
    //overview: Choose to enter game or view help
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button->text() == welcomePage1Str[0]) {
        welcomeWindow->setCurrentIndex(1);
    } else if (button->text() == welcomePage1Str[1]) {
        welcomeWindow->setCurrentIndex(2);
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
        if (currentX != -1 && currentY != -1) { // Uncheck already checked button
            puzzleButtons[currentX][currentY]->setChecked(false);
        } 

        if (currentX == x && currentY == y) { // Unchecked button is the pressed button
            currentPositionSet(-1, -1);
        } else { // Button is unchecked
            currentPositionSet(x, y);
        }
    } else {
        currentPositionSet(-1, -1);
    }
}

void Sudoku_GUI::pressButtonChoice() {
    /*
    @overview:invoked if choice button clicked
    */
    if (currentX == -1 && currentY == -1) {
        return;
    } else {
        QPushButton *button = qobject_cast<QPushButton*>(sender());
        int name = button->text().toInt();
        puzzleButtons[currentX][currentY]->setText(QString::number(name));
        checkGame();
        puzzleButtons[currentX][currentY]->setChecked(false); // Set button unchecked
        currentPositionSet(-1, -1);
    }
}

void Sudoku_GUI::pressButtonHint() {
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
    } else {
        QMessageBox::information(this, tr("Bad Sudoku"), tr("Can not give a hint.The current Sudoku\
 is not valid\nPlease check the row,rolumn or 9x9 block to correct it."));
    }

}

void Sudoku_GUI::pressButtonReturn() {
    welcomeWindow->setCurrentIndex(0);
}

void Sudoku_GUI::pressButtonDisplace() {
    /*
    @overview:invoked when displace button clicked, displace number in selected
    button.
    */
    QString nullStr = QString("");
    puzzleButtons[currentX][currentY]->setText(nullStr);
    puzzleButtons[currentX][currentY]->setChecked(false);
    checkGame();
}

void Sudoku_GUI::pressMenuButtonNewGame() {
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

void Sudoku_GUI::pressMenuButtonSave() {
    QAction* action = qobject_cast<QAction*>(sender());
    for (int i = 0; i < SAVE_CHOICE_COUNT; i++) {
        if (action->text() == saveChoices[i]) {
            saveDataAtIndex(i);
            break;
        }
    }
}

void Sudoku_GUI::pressMenuButtonLoad() {
    QAction* action = qobject_cast<QAction*>(sender());
    for (int i = 0; i < SAVE_CHOICE_COUNT; i++) {
        if (action->text() == loadChoices[i]) {
            loadDataAtIndex(i);
            break;
        }
    }
}