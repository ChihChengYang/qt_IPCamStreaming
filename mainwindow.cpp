#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initDISWindow(int w,int h)
{

    this->setWindowTitle(" ");

    thread = new QThread();

    QRect screenRect = QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen());


    int ww = w+1;
    int wh = h+10;
    this->setGeometry(screenRect.center().x()-(ww*0.5),screenRect.center().y()-(wh*0.5),
                     ww,wh);

    disWindow = new DISWindow();
    disWindow->initWindow(w , h);
   // disWindow->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
   // disWindow->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    disWindow->setGeometry(0, 0, ww ,wh);

    connect(disWindow, SIGNAL(workRequested()), thread, SLOT(start()));
    connect(thread, SIGNAL(started()), disWindow, SLOT(doWork()));
    connect(disWindow, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);

    disWindow->startThread();

    this->setCentralWidget( disWindow );
}
