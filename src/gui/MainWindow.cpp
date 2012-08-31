#include "gui/MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->GLwidget->grabKeyboard();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QWidget *MainWindow::GetGLWidget()
{
    return ui->GLwidget;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    std::cout << "main window key press" << std::endl;

    //ui->GLwidget->keyPressEvent(event);
}


void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    std::cout << "main window key release" << std::endl;

  //  ui->GLwidget->keyReleaseEvent(event);
}
