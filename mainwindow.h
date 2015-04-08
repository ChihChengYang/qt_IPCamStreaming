#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QThread>
#include <QAction>
#include "DISWindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initDISWindow(int w,int h);

    QThread *thread;

    DISWindow *disWindow;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
