#ifndef NemoFaceRecogWidget_H
#define NemoFaceRecogWidget_H

#include <QTableWidget>
#include <QString>

#include "SafeQueue.hpp"
#include "faceviewwidget.h"

#include "arcfaceengine.h"

class NemoFaceRecogWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit NemoFaceRecogWidget(QWidget *parent = 0);
    ~NemoFaceRecogWidget();

    FaceViewWidget *nemoFaceWidget[4];
    int nemoFaceWidgetIndex;

    void setArcFaceEngine(ArcFaceEngine *engine)
    {
        mArcFaceEngine = engine;
    }
protected:

    SafeQueue<std::string> fileQueue;
    bool inotifyRunning;
    std::thread *inotify_thread;
    std::thread *fr_thread;
    static void inotifyThreadProc(void *args);
    static void frThreadProc(void *args);

public:
    void doFaceRecognition();
    void showNemoFace(QString name, QImage image);

signals:
    void updateFaceRecogResult(QImage img, int faceIndex, float score);

public slots:
    void ShowFaceRecogResult(QImage img, int faceIndex, float score);

private:
    ArcFaceEngine *mArcFaceEngine;
    std::string faceImageDir;
};

#endif // NemoFaceRecogWidget_H
