/*
#if _MSC_VER >= 1600
#pragma execution_character_set("gbk")
#endif
*/
//#pragma comment(lib,"SudokuDll.lib")
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStackedWidget"
#include "QVBoxLayout"
#include "QPushButton"
#include "QLabel"
#include "QGridLayout"
#include "QHBoxLayout"
#include "../GUI/Sudoku.h"
#include <iostream>


static QString welcomePage1Str[2] = {"NewGame","help"};
static QString welcomePage2Str[3] = {"Easy","Medium","Hard"};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    sudoku(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("数独游戏"));

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
    QLabel* welcomeLabel = new QLabel(tr("数独游戏"));
    welcomeLabel->setAlignment(Qt::AlignHCenter);
    welcomePage1Layout->addWidget(welcomeLabel);
    welcomePage1Layout->addStretch(2);
    welcomePage1Layout->setMargin(180);
    for(int i = 0 ; i < 2 ; ++ i){
        QPushButton *button = new QPushButton(welcomePage1Str[i]);
        welcomePage1Layout->addWidget(button);
        connect(button,&QPushButton::clicked,this,&MainWindow::pressButtonWelcome);
        welcomePage1Layout->addStretch(1);
    }


    //welcome window page 2
    QVBoxLayout* welcomePage2Layout = new QVBoxLayout(welcomeWidgetPage2);
    QLabel* chooseDifficultyLabel = new QLabel(tr("选择难度"));
    chooseDifficultyLabel->setAlignment(Qt::AlignHCenter);
    welcomePage2Layout->addWidget(chooseDifficultyLabel);
    welcomePage2Layout->addStretch(2);
    welcomePage2Layout->setMargin(180);
    for(int i = 0 ; i < 3 ; ++i){
        QPushButton *button = new QPushButton(welcomePage2Str[i]);
        connect(button,&QPushButton::clicked,this,&MainWindow::pressButtonDifficulty);
        welcomePage2Layout->addWidget(button);
        welcomePage2Layout->addStretch(1);
    }

    //Main game page
    QVBoxLayout* mainLayout = new QVBoxLayout(gameWindow);
    QGridLayout* puzzleLayout = new QGridLayout(gameWindow);
    QHBoxLayout* choicesLayout = new QHBoxLayout(gameWindow);
    //Add puzzle buttons
    puzzleButtons = new QPushButton**[9];
    for(int i = 0; i < 9; i++) {
        puzzleButtons[i] = new QPushButton*[9];
        for(int j = 0; j < 9; j++) {
            puzzleButtons[i][j] = new QPushButton();
            puzzleButtons[i][j]->setMinimumSize (30,30);
            puzzleButtons[i][j]->setStyleSheet("padding:-1");
            puzzleLayout->addWidget (puzzleButtons[i][j], i, j);
            connect (puzzleButtons[i][j], &QPushButton::clicked, this, &MainWindow::pressButtonPuzzle);
            puzzleButtons[i][j]->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
        }
    }
    puzzleLayout->setMargin(80);
    puzzleLayout->setVerticalSpacing(0);
    puzzleLayout->setHorizontalSpacing(0);
    for(int i = 0; i < 9; ++i){
        puzzleLayout->setColumnStretch(i,1);
        puzzleLayout->setRowStretch(i,1);
        puzzleLayout->setColumnMinimumWidth (i, 20);
    }

    //Add choices buttons
    QPushButton* choicesButtons[9];
    for(int i = 0; i < 9; i++) {
        choicesButtons[i] = new QPushButton();
        choicesLayout->addWidget (choicesButtons[i], 1);
    }
    mainLayout->addLayout (puzzleLayout, 0);
    mainLayout->addLayout (choicesLayout, 0);

    this->setMinimumSize(QSize(1000,1000));
    this->setMaximumSize(QSize(1000,1000));

}

void MainWindow::pressButtonWelcome() {
    //overview: Choose to enter game or view help
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button->text () == welcomePage1Str[0]) {
        welcomeWindow->setCurrentIndex (1);
    } else if (button->text () == welcomePage1Str[1]) {
        //welcomeWindow->setCurrentIndex (2);
    }
}


void MainWindow::pressButtonDifficulty() {
    //overview: Switch to game view when difficulty button pressed
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    int i;
    for(i = 0;i < 3; ++i) {
        if(button->text() == welcomePage2Str[i]){
            break;
        }
    }
    mainWindow->setCurrentIndex (1);
    //Generate sudoku puzzle with different difficulty
    gameSet(i+1);
}

void MainWindow::pressButtonPuzzle() {
    //overview: Invoked when puzzle buttons pressed
}

void MainWindow::gameSet(int degOfDifficulty){
    sudoku = new Sudoku();
    int result[10][LEN*LEN];
    sudoku->generate(10,degOfDifficulty,result);
    srand((unsigned)time(nullptr));
    int target = rand()%10;
    QString temp;
    for(int i = 0 ; i < LEN*LEN ; ++i){
        for(int j = 0 ; j < LEN*LEN ; ++j){
            if(result[target][i*LEN+j] == 0){
                puzzleButtons[i][j]->setText("");
            }
            else{
                puzzleButtons[i][j]->setText(temp.setNum(result[target][i*LEN+j]));
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

