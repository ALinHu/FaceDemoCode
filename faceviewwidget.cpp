#include "faceviewwidget.h"
#include <QtWidgets>

FaceViewWidget::FaceViewWidget(QWidget *parent,int id,QString name,QImage image) : QDialog(parent)
{
    mID = id;
    mName = name;

    setWindowTitle(mName);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    pLabel = new QLabel;
    pLabel->setPixmap(QPixmap::fromImage(image).scaled(256,256,Qt::KeepAspectRatio));
    mainLayout->addWidget(pLabel);
    setLayout(mainLayout);
}


void FaceViewWidget::onBroadCast(int id,float fScore)
{
    if(id == mID){
        setWindowTitle(mName+"   "+QString::number(fScore));
        activateWindow();
    }else{
        setWindowTitle(mName);
    }
}

void FaceViewWidget::closeEvent(QCloseEvent *event)
{
    emit removeFaceSignal(mID);
    event->accept();
}

void FaceViewWidget::SetNewFace(int id,QString name,QImage image)
{
    mID = id;
    mName = name;
    setWindowTitle(mName);
    pLabel->setPixmap(QPixmap::fromImage(image).scaled(256,256,Qt::KeepAspectRatio));
    repaint();
}
