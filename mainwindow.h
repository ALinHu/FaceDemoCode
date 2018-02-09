#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "arcfaceengine.h"
#include <QMap>
#include <QSound>

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
    //void on_btnRegCamera_clicked();
    //void on_btnRegImage_clicked();
    //void on_btnFaceRecog_clicked();

    //FileAdd
    void timerUpdate1();
    void timerUpdate2();

private:
    Ui::MainWindow *ui;
    ArcFaceEngine *arcFaceEngine;

    QMap<QString,QStringList> currentContent;
    QSound *sound;
};

#endif // MAINWINDOW_H
