/*
#if _MSC_VER >= 1600
#pragma execution_character_set("gbk")
#endif
*/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStackedWidget"
#include "QVBoxLayout"
#include "QPushButton"
#include "QLabel"
#include <iostream>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("数独游戏"));

    //widget
    mainWindow = new QStackedWidget();
    welcomeWindow = new QStackedWidget();
    mainWindow->addWidget(welcomeWindow);
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
    QString welcomePage1Str[2] = {tr("NewGame"),tr("help")};
    for(int i = 0 ; i < 2 ; ++ i){
        QPushButton *button = new QPushButton(welcomePage1Str[i]);
        welcomePage1Layout->addWidget(button);
        //connect(button,&QPushButton::clicked,this,&MainWindow::);
        welcomePage1Layout->addStretch(1);
    }


    //welcome window page 2
    QVBoxLayout* welcomePage2Layout = new QVBoxLayout(welcomeWidgetPage2);
    QLabel* chooseDifficultyLabel = new QLabel(tr("选择难度"));
    chooseDifficultyLabel->setAlignment(Qt::AlignHCenter);
    welcomePage2Layout->addWidget(chooseDifficultyLabel);
    welcomePage2Layout->addStretch(2);
    welcomePage2Layout->setMargin(180);
    QString welcomePage2Str[3] = {tr("Easy"),tr("Medium"),tr("Hard")};
    for(int i = 0 ; i < 3 ; ++i){
        QPushButton *button = new QPushButton(welcomePage2Str[i]);
        welcomePage2Layout->addWidget(button);
        welcomePage2Layout->addStretch(1);
    }
    welcomeWindow->setCurrentIndex(1);


}

MainWindow::~MainWindow()
{
    delete ui;
}

