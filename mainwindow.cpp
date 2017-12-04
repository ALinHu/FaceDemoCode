#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    arcFaceEngine = new ArcFaceEngine();
    ui->cameraVideoWidget->setArcFaceEngine(arcFaceEngine);
    ui->faceRecogWidget->setArcFaceEngine(arcFaceEngine);
    ui->faceStillImageView->setArcFaceEngine(arcFaceEngine);

    arcFaceEngine->loadFaceDB();

    on_btnFaceRecog_clicked();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnRegCamera_clicked()
{
    ui->cameraVideoWidget->show();
    ui->faceStillImageView->hide();
    ui->faceRecogWidget->hide();

    ui->cameraVideoWidget->registerFaceFromCamera();
}

void MainWindow::on_btnRegImage_clicked()
{
    ui->cameraVideoWidget->hide();
    ui->faceStillImageView->show();
    ui->faceRecogWidget->hide();
}

void MainWindow::on_btnFaceRecog_clicked()
{
    ui->cameraVideoWidget->closeCamera();

    ui->cameraVideoWidget->hide();
    ui->faceStillImageView->hide();
    ui->faceRecogWidget->show();

    ui->faceRecogWidget->doFaceRecognition();
}


