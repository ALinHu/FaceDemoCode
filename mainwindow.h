#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "arcfaceengine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnRegCamera_clicked();
    void on_btnRegImage_clicked();
    void on_btnFaceRecog_clicked();

private:
    Ui::MainWindow *ui;
    ArcFaceEngine *arcFaceEngine;
};

#endif // MAINWINDOW_H
