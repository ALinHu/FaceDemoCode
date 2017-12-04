#ifndef FACESTILLIMAGEVIEW_H
#define FACESTILLIMAGEVIEW_H

#include <QGraphicsView>
#include "arcfaceengine.h"


class FaceStillImageView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit FaceStillImageView(QWidget *parent = 0);
    ~FaceStillImageView();

    void setArcFaceEngine(ArcFaceEngine *engine)
    {
        mArcFaceEngine = engine;
    }

private:
    ArcFaceEngine *mArcFaceEngine;

};

#endif // FACESTILLIMAGEVIEW_H
