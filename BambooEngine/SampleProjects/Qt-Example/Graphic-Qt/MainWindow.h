#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Graphics/Graphic.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateGL();

private:
    Ui::MainWindow *ui;
    Bamboo *m_pBamboo;
    std::shared_ptr<Bamboo::IRenderTarget> m_spGLWidget;
    std::shared_ptr<Bamboo::IRenderTarget> m_spGLWidget2;
};

#endif // MAINWINDOW_H
