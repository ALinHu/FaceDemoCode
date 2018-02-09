#include "mainwindow.h"
#include "ui_mainwindow.h"

//FileAdd
#include <QDir>
#include <QTime>
#include <QTimer>
#include <QSound>
#include <QFileDialog>
#include <QTextStream>
QString path = "/home/deepglint/ftp_svr_dir/1018009415";
QString dspath = "/home/deepglint/ftp_svr_dir/1018009415/q";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    arcFaceEngine = new ArcFaceEngine();
    ui->faceRecogWidget->setArcFaceEngine(arcFaceEngine);
    ui->faceRecogWidget->doFaceRecognition();
    arcFaceEngine->loadFaceDB();

    //FileAdd
    sound = new QSound("/home/deepglint/Music/189.wav",this);

    QTimer *timer1 = new QTimer(this);
    connect(timer1,&QTimer::timeout,this,&MainWindow::timerUpdate1);
    timer1->start(1000);

    QTimer *timer2 = new QTimer(this);
    connect(timer2,&QTimer::timeout,this,&MainWindow::timerUpdate2);
    timer2->start(60000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//FileAdd
void MainWindow::timerUpdate1()
{
    QDir dir(path);
    QStringList currEntryList = currentContent[path];
    QStringList newEntryList = dir.entryList(QDir::NoDotAndDotDot  |
                           QDir::AllDirs | QDir::Files, QDir::DirsFirst);
    QSet<QString> curDirSet = QSet<QString>::fromList(currEntryList);
    QSet<QString> newDirSet = QSet<QString>::fromList(newEntryList);
    QSet<QString> newFiles = newDirSet - curDirSet;
    QStringList newFile = newFiles.toList();
    currentContent[path] = newEntryList;
    if(!newFile.isEmpty())
    {
        sound->play();
    }
}

void MainWindow::timerUpdate2()
{
    //------copy and delete dir------//
    QDir dir(path);
    QStringList excludeFiles;
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();
    int count = list.count();
    for(int i=0;i<count;i++)
    {
        QFileInfo fileInfo = list.at(i);
        QString fileName = fileInfo.fileName();
        if(excludeFiles.indexOf(fileName)!=-1)
            continue;
        if(!dir.exists(dspath))
            dir.mkpath(dspath);
        QString newSrcFileName = path + "/" + fileInfo.fileName();
        QString newDstFileName = dspath+ "/" + fileInfo.fileName();
        QFile(newSrcFileName).copy(newDstFileName);
    }
    for(int j = 0;j < count;j++)
        dir.remove(dir[j]);
}
