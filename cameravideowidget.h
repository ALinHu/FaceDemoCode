#ifndef CAMERAVIDEOWIDGET_H
#define CAMERAVIDEOWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

#include "SafeQueue.hpp"
#include "faceviewwidget.h"

class VideoFileSource;
class CameraSource;
class GLHelper;

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram);
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

#include "arcfaceengine.h"

class CameraVideoWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit CameraVideoWidget(QWidget *parent = 0);
    ~CameraVideoWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void setArcFaceEngine(ArcFaceEngine *engine)
    {
        mArcFaceEngine = engine;
    }

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    bool pointInFaceRect(int x, int y, MRECT faceRect);
    void mouseDoubleClickEvent(QMouseEvent * event) override;


public slots:
    void registerFaceFromCamera();
    void registerFaceFromImage();

    void openCamera();
    void closeCamera();
    void openVideoFile();

    void registerFace();
    void removeFace(int id);

signals:
    void recognBroadCastSignal(int id,float fscore);
private:

    GLHelper* mGLHelper;
    CameraSource *mCameraSource;
    VideoFileSource *mVideoFileSource;
    bool bUseStillImage;

    unsigned char *mFrameData;
    int mFrameDataSize;
    int mFrameWidth;
    int mFrameHeight;
    int mFrameFormat;
    bool bFrameMirrored;


    unsigned char *mWidgetData;
    int mWidgetWidth;
    int mWidgetHeight;

    ArcFaceEngine *mArcFaceEngine;

    bool bWaitForInputName;
    int mFaceWidth;
    int mFaceHeight;
};

#endif // CAMERAVIDEOWIDGET_H
