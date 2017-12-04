#include "cameravideowidget.h"
#include "faceviewwidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <QDebug>
#include <QInputDialog>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>

#include "camerasource.h"
#include "videofilesource.h"
#include "glhelper.h"
#include "arcfaceengine.h"
#include "asvloffscreen.h"

#include "rgb2yuv.h"



CameraVideoWidget::CameraVideoWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    mFaceWidth = 0;
    mFaceHeight = 0;

    bWaitForInputName = false;
    bFrameMirrored = false;
    mCameraSource = nullptr;
    mVideoFileSource = nullptr;

    mFrameData = nullptr;

    bUseStillImage = false;

    mGLHelper = nullptr;
    mWidgetData = nullptr;

    mArcFaceEngine = new ArcFaceEngine();


//    setFixedSize(640, 360);


}

CameraVideoWidget::~CameraVideoWidget()
{
    if(mArcFaceEngine){
        delete mArcFaceEngine;
        mArcFaceEngine = nullptr;
    }

    if(mWidgetData){
        delete[] mWidgetData;
        mWidgetData = nullptr;
    }

    if(mFrameData){
        delete[] mFrameData;
        mFrameData = nullptr;
    }

    if(mCameraSource != nullptr){
        delete mCameraSource;
        mCameraSource = nullptr;
    }

    if(mVideoFileSource != nullptr){
        delete mVideoFileSource;
        mVideoFileSource = nullptr;
    }

    if (mGLHelper != nullptr) {
        mGLHelper->uninit();
        delete mGLHelper;
        mGLHelper = nullptr;
    }


}

void CameraVideoWidget::registerFaceFromCamera()
{
    openCamera();
}

void CameraVideoWidget::registerFaceFromImage()
{
#ifdef WIN32
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image File"),QDir::homePath());
#else
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image File"),
                                                    QDir::homePath(),tr("Images (*.png *.bmp *.jpg)"),nullptr,
                                                    QFileDialog::DontUseNativeDialog);
#endif

    if (fileName.isEmpty())
    {
        return;
    }
    else
    {
        QImage img;
        if(!(img.load(fileName))) //加载图像
        {
            QMessageBox::information(this, tr("打开图像失败"), tr("打开图像失败!"));
            return;
        }
        img = img.convertToFormat(QImage::Format_RGB888);


        mCameraSource = nullptr;
        mVideoFileSource = nullptr;

        if (mFrameData)
            delete mFrameData;

        mFrameWidth = img.width();
        mFrameHeight = img.height();
        mFrameDataSize = mFrameWidth * mFrameHeight * 3 / 2;
        mFrameData = new unsigned char[mFrameDataSize];
        mFrameFormat = ASVL_PAF_I420;
        //   memcpy(mFrameData, img.bits(), mFrameDataSize);
        // rgb24_to_yuv420(mFrameWidth, mFrameHeight, img.bits(), mFrameData, 0);
        rgb24_to_yuv420p(mFrameData, mFrameData + mFrameWidth * mFrameHeight,mFrameData +  mFrameWidth * mFrameHeight * 5 /4, img.bits(), mFrameWidth, mFrameHeight);

        bUseStillImage = true;

        if(mFrameWidth>mFrameHeight){
            mWidgetWidth = 640;
            mWidgetHeight = mFrameHeight*mWidgetWidth/mFrameWidth;
        }else{
            mWidgetHeight = 640;
            mWidgetWidth = mFrameWidth*mWidgetHeight/mFrameHeight;
        }

        mWidgetData = new unsigned char[mWidgetWidth*mWidgetHeight * 4];
        setFixedSize(mWidgetWidth, mWidgetHeight);
    }
}



void CameraVideoWidget::openCamera(){
    if(mVideoFileSource != nullptr){
        return;
    }

    if(mCameraSource == nullptr){

        bFrameMirrored = true;
#ifdef WIN32
        mCameraSource = new CameraSource(640, 480, ASVL_PAF_I420);
#else
        mCameraSource = new CameraSource(640, 480, ASVL_PAF_YUYV);
#endif
        mCameraSource->Open(0,"");
    }
}

void CameraVideoWidget::closeCamera()
{
    //openCamera();
    if (mCameraSource)
    {
        mCameraSource->Close();
        delete mCameraSource;
        mCameraSource = nullptr;
    }
}

void CameraVideoWidget::openVideoFile(){
    if(mCameraSource != nullptr){
        return;
    }

    if(mVideoFileSource == nullptr){
#ifdef WIN32
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Video File"),QDir::homePath());
#else
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Video File"),
                                                        QDir::homePath(),"",nullptr,
                                                        QFileDialog::DontUseNativeDialog);
#endif
        if (!fileName.isEmpty()) {
            bFrameMirrored = false;
            mVideoFileSource = new VideoFileSource();
            mVideoFileSource->Open(0,fileName);
        }
    }

}

void CameraVideoWidget::registerFace(){
    if(mArcFaceEngine){
        if(mArcFaceEngine->mFaceNum > 0){
            int id = -1;
            if(0 == mArcFaceEngine->registerFace(mFrameData,mFrameWidth,mFrameHeight,mFrameFormat,&mArcFaceEngine->mFaceRect[0],mArcFaceEngine->mFaceOrient[0],&id)){
                bWaitForInputName = true;
                bool bGetFaceName = false;
                QString defaultName = "N_"+ QString::number(mArcFaceEngine->mUniqueIncID);
                QString name = QInputDialog::getText(NULL, "Register Face",
                                                     "Please input name of face",
                                                     QLineEdit::Normal,
                                                     defaultName,
                                                     &bGetFaceName,windowFlags());
                bWaitForInputName = false;
                if(bGetFaceName) {
                    mArcFaceEngine->updateFaceName(id,name);

                    QImage faceimage(mWidgetData,mFaceWidth, mFaceHeight, QImage::Format_RGB32);

                    faceimage = faceimage.mirrored(false,true);
                    FaceViewWidget *faceview = new FaceViewWidget(this,id,name,faceimage);
                    connect(this, &CameraVideoWidget::recognBroadCastSignal, faceview, &FaceViewWidget::onBroadCast);
                    connect(faceview, &FaceViewWidget::removeFaceSignal,this, &CameraVideoWidget::removeFace);
                    faceview->show();
                }else{
                    mArcFaceEngine->removeFace(id);
                }
            }
        }
    }
}

void CameraVideoWidget::removeFace(int id){
    if(mArcFaceEngine){
        mArcFaceEngine->removeFace(id);
    }
}

QSize CameraVideoWidget::minimumSizeHint() const
{
    return QSize(mWidgetWidth, mWidgetHeight);
}

QSize CameraVideoWidget::sizeHint() const
{
    return QSize(mWidgetWidth, mWidgetHeight);
}

void CameraVideoWidget::initializeGL()
{
    if (mGLHelper == NULL) {
        mGLHelper = new GLHelper();
    }

    initializeOpenGLFunctions();
    glClearDepthf(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glLineWidth(1.0);
    glEnable(GL_MULTISAMPLE);

}

void CameraVideoWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QPainter painter(this);
    painter.setPen(Qt::green);
    painter.setFont(QFont("Arial", 10));
    painter.beginNativePainting();
    bool bNativePainting = true;

    FrameSource *frameSource = nullptr;

    if (mCameraSource != nullptr) {
        frameSource = mCameraSource;
    }else if (mVideoFileSource != nullptr){
        frameSource = mVideoFileSource;
    }

    if( ((frameSource != nullptr)&&(frameSource->Ready()))  || (bUseStillImage == true)){
        if(mFrameData == nullptr){
            mFrameFormat = frameSource->Format();
            mFrameWidth = frameSource->Width();
            mFrameHeight = frameSource->Height();

            if ((mFrameFormat == ASVL_PAF_NV21) || (mFrameFormat == ASVL_PAF_NV12) || (mFrameFormat == ASVL_PAF_I420)) {
                mFrameDataSize = mFrameWidth*mFrameHeight * 3 / 2;
            }else if(mFrameFormat == ASVL_PAF_YUYV){
                mFrameDataSize = mFrameWidth*mFrameHeight *2;
            }else {
                mFrameDataSize = mFrameWidth*mFrameHeight *4;
            }

            mFrameData = new unsigned char[mFrameDataSize];
            if(mFrameWidth>mFrameHeight){
                mWidgetWidth = 640;
                mWidgetHeight = mFrameHeight*mWidgetWidth/mFrameWidth;
            }else{
                mWidgetHeight = 640;
                mWidgetWidth = mFrameWidth*mWidgetHeight/mFrameHeight;
            }

            mWidgetData = new unsigned char[mWidgetWidth*mWidgetHeight * 4];
            setFixedSize(mWidgetWidth, mWidgetHeight);
        }else{
            if (!bUseStillImage)
            {
                frameSource->ReadFrame(mFrameData,mFrameDataSize);
            }
            else
            {
                bFrameMirrored = false;

            }

            mGLHelper->setMirror(bFrameMirrored);

            mGLHelper->drawTexture(mFrameData, mFrameFormat, mFrameWidth, mFrameHeight);

            if(mArcFaceEngine->processFrame(mFrameData,mFrameWidth,mFrameHeight,mFrameFormat)){

                for (int i = 0; i < mArcFaceEngine->mFaceNum; i++) {

                    float rectPoints[8];
                    MRECT *rect = &mArcFaceEngine->mFaceRect[i];
                    rectPoints[0] = (float)rect->left;
                    rectPoints[1] = (float)rect->top;
                    rectPoints[2] = (float)rect->right;
                    rectPoints[3] = (float)rect->top;
                    rectPoints[4] = (float)rect->right;
                    rectPoints[5] = (float)rect->bottom;
                    rectPoints[6] = (float)rect->left;
                    rectPoints[7] = (float)rect->bottom;

                    if(i == 0){
                        if(!bWaitForInputName){
                            int leftInWidget = mArcFaceEngine->mFaceRect[i].left*mWidgetWidth/mFrameWidth;
                            int topInWidget = mArcFaceEngine->mFaceRect[i].top*mWidgetHeight/mFrameHeight;
                            int rightInWidget = mArcFaceEngine->mFaceRect[i].right*mWidgetWidth/mFrameWidth;
                            int bottomInWidget = mArcFaceEngine->mFaceRect[i].bottom*mWidgetHeight/mFrameHeight;

                            mFaceWidth = rightInWidget-leftInWidget;
                            mFaceHeight = bottomInWidget-topInWidget;
                            if(bFrameMirrored){
                                glReadPixels(mWidgetWidth-1-rightInWidget,mWidgetHeight-1-bottomInWidget, mFaceWidth, mFaceHeight, GL_BGRA, GL_UNSIGNED_BYTE, mWidgetData);
                            }else{
                                glReadPixels(leftInWidget,mWidgetHeight-1-bottomInWidget, mFaceWidth, mFaceHeight, GL_BGRA, GL_UNSIGNED_BYTE, mWidgetData);
                            }
                        }
                    }

                    mGLHelper->drawPoints(4, rectPoints, mFrameWidth, mFrameHeight, 1.0f, 1.0f, 0.0f, 1.0f, GL_LINE_LOOP);
                }

                painter.endNativePainting();
                bNativePainting = false;
                for (int i = 0; i < mArcFaceEngine->mFaceNum; i++) {
                    if(mArcFaceEngine->mFaceID[i]>=0){
                        int leftInWidget = mArcFaceEngine->mFaceRect[i].left*mWidgetWidth/mFrameWidth;
                        int topInWidget = mArcFaceEngine->mFaceRect[i].top*mWidgetHeight/mFrameHeight;
                        int rightInWidget = mArcFaceEngine->mFaceRect[i].right*mWidgetWidth/mFrameWidth;

                        if(bFrameMirrored){
                            painter.drawText(QPointF(mWidgetWidth-1-rightInWidget,topInWidget), mArcFaceEngine->mFaceName[i]);
                        }else{
                            painter.drawText(QPointF(leftInWidget,topInWidget), mArcFaceEngine->mFaceName[i]);
                        }
                    }
                }
            }
        }
    }

    if(bNativePainting){
        painter.endNativePainting();
    }
    update();
}

void CameraVideoWidget::resizeGL(int width, int height)
{
    glViewport(mWidgetWidth, mWidgetHeight, 0, 0);
}


bool CameraVideoWidget::pointInFaceRect(int x, int y, MRECT faceRect)
{
    int leftInWidget    = faceRect.left   * mWidgetWidth  / mFrameWidth;
    int topInWidget     = faceRect.top    * mWidgetHeight / mFrameHeight;
    int rightInWidget   = faceRect.right  * mWidgetWidth  / mFrameWidth;
    int bottomInWidget  = faceRect.bottom * mWidgetHeight / mFrameHeight;

    mFaceWidth = rightInWidget - leftInWidget;

    if (bFrameMirrored)
    {
        leftInWidget = mWidgetWidth - 1 - rightInWidget;
        rightInWidget = leftInWidget + mFaceWidth;
    }

    qDebug() << x << "," << y << "(" << leftInWidget <<  "," << topInWidget <<  "," << rightInWidget <<  "," << bottomInWidget  << ")";
    return (y >= topInWidget && y < bottomInWidget && x >= leftInWidget && x < rightInWidget);
}

// double click to register face
void CameraVideoWidget::mouseDoubleClickEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        if(mArcFaceEngine){
            if(mArcFaceEngine->mFaceNum > 0){
                int i;
                for (i = 0; i < mArcFaceEngine->mFaceNum; i++)
                {
                    if (pointInFaceRect(event->x(), event->y(), mArcFaceEngine->mFaceRect[i]))
                        break;
                }

                if (i >= mArcFaceEngine->mFaceNum)
                    return;

                int id = -1;
                if(0 == mArcFaceEngine->registerFace(mFrameData,mFrameWidth,mFrameHeight,mFrameFormat,&mArcFaceEngine->mFaceRect[i],mArcFaceEngine->mFaceOrient[i],&id)){
                    bWaitForInputName = true;
                    bool bGetFaceName = false;
                    QString defaultName = "N_"+ QString::number(mArcFaceEngine->mUniqueIncID);
                    QString name = QInputDialog::getText(NULL, "Register Face",
                                                         "Please input name of face",
                                                         QLineEdit::Normal,
                                                         defaultName,
                                                         &bGetFaceName,windowFlags());
                    bWaitForInputName = false;
                    if(bGetFaceName) {
                        mArcFaceEngine->updateFaceName(id,name);

                        QImage faceimage(mWidgetData,mFaceWidth, mFaceHeight, QImage::Format_RGB32);

                        faceimage = faceimage.mirrored(false,true);
                        FaceViewWidget *faceview = new FaceViewWidget(this,id,name,faceimage);
                        connect(this, &CameraVideoWidget::recognBroadCastSignal, faceview, &FaceViewWidget::onBroadCast);
                        connect(faceview, &FaceViewWidget::removeFaceSignal,this, &CameraVideoWidget::removeFace);
                        faceview->show();
                    }else{
                        mArcFaceEngine->removeFace(id);
                    }
                }
            }
        }
    }
}


