#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QStackedWidget;
class QWidget;
class QPushButton;
class Sudoku;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    void pressButtonWelcome();
    void pressButtonDifficulty();
    void pressButtonPuzzle();
    ~MainWindow();

private slots:


private:
    Ui::MainWindow *ui;
    QStackedWidget* mainWindow;
    QStackedWidget* welcomeWindow;
    QWidget* gameWindow;
    QPushButton*** puzzleButtons;
    Sudoku* sudoku;

    void gameSet(int degOfDiffculty);

};

#endif // MAINWINDOW_H
