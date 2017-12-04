#ifndef FACEWIDGET_H
#define FACEWIDGET_H

#include <QDialog>
#include <QLabel>

class FaceViewWidget : public QDialog
{
    Q_OBJECT
public:
    explicit FaceViewWidget(QWidget *parent,int id,QString name,QImage image);
    void SetNewFace(int id,QString name,QImage image);

signals:
    void removeFaceSignal(int id);
public slots:
   void onBroadCast(int id,float fScore);

protected:
   void closeEvent(QCloseEvent *event);
private:
   int mID;
   QString mName;
   QLabel *pLabel;

};

#endif // FACEWIDGET_H
