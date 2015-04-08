#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <QDebug>
using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    int width = 1280;
    int height = 720;


     w.initDISWindow(width,height);
     w.show();

    return a.exec();
}
