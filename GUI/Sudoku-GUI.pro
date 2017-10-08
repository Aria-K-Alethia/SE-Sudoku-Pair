#-------------------------------------------------
#
# Project created by QtCreator 2017-10-08T11:39:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sudoku-GUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

#LIBS += -LC:\Users\Lenovo\source\repos\SE-Sudoku\GUI -lSuodkuDll
LIBS += "C:\Users\Lenovo\source\repos\SE-Sudoku\GUI\SudokuDll.lib"
