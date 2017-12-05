#include "nemofacerecogwidget.h"
#include <QMessageBox>
#include <QHeaderView>

#include <unistd.h>
#include <sys/inotify.h>
#include <fcntl.h>

#include "rgb2yuv.h"

#include <QDebug>
#include <QSettings>

NemoFaceRecogWidget::NemoFaceRecogWidget(QWidget *parent)
    : QTableWidget(parent)
{

    inotifyRunning = false;

    faceImageDir = "";

    QSettings user_cfg("config.ini",QSettings::IniFormat);
    user_cfg.beginGroup("config");
    faceImageDir = user_cfg.value("ImageDir").toString().toStdString();
    user_cfg.endGroup();

    qDebug() << "faceImageDir: " << faceImageDir.c_str();


    nemoFaceWidgetIndex = -1;
    for (int i = 0; i < 4; i++)
        nemoFaceWidget[i] = nullptr;

    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(this, SIGNAL(updateFaceRecogResult(QImage, int, float)),   this, SLOT(ShowFaceRecogResult(QImage, int, float)));
}

NemoFaceRecogWidget::~NemoFaceRecogWidget()
{

}

void NemoFaceRecogWidget::doFaceRecognition()
{
    if (inotifyRunning)
    {
        inotifyRunning = false;
        fr_thread->join();
        delete fr_thread;
        inotify_thread->join();
        delete inotify_thread;

    }
    else
    {
        inotifyRunning = true;
        inotify_thread = new std::thread(inotifyThreadProc, this);
        fr_thread = new std::thread(frThreadProc, this);

    }


    //frThread.join();
    //inotifyThread.join();


}

#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))
void NemoFaceRecogWidget::inotifyThreadProc(void *args)
{
    int length, i = 0;
    int fd;
    int wd;
    char buffer[EVENT_BUF_LEN];

    /*creating the INOTIFY instance*/
    fd = inotify_init();

    /*checking for error*/
    if (fd < 0)
    {
        perror("inotify_init");
    }

    int fd_flag = 0;
    fd_flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, fd_flag | O_NONBLOCK);


    /*adding the “/tmp” directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
    wd = inotify_add_watch(fd, ((NemoFaceRecogWidget*)args)->faceImageDir.c_str(), IN_CREATE | IN_DELETE | IN_MOVED_TO);

    while (((NemoFaceRecogWidget*)args)->inotifyRunning)
    {
        length = ::read(fd, buffer, EVENT_BUF_LEN);

        /*checking for error*/
        if (length <= 0)
        {
            //fprintf(stderr, "length < 0.\n");
            //perror("read");
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }
        i = 0;
        /*actually read return the list of change events happens. Here, read the change event one by one and process it accordingly.*/
        while (i < length)
        {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            if (event->len)
            {
                if (event->mask & IN_CREATE)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        fprintf(stderr, "Directory %s IN_CREATE.\n", event->name);
                    }
                    else
                    {
                        fprintf(stderr, "File %s IN_CREATE.\n", event->name);

                        std::string fileName = event->name;
                        const char *ext = strrchr(fileName.c_str(), '.');
                        string sext(ext+1);
                        if (sext.compare("jpg") == 0 || sext.compare("JPG") == 0)
                            ((NemoFaceRecogWidget*)args)->fileQueue.push(fileName);
                    }
                }
                else if (event->mask & IN_MOVED_TO)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        fprintf(stderr, "Directory %s IN_MOVED_TO.\n", event->name);
                    }
                    else
                    {
                        //fprintf(stderr, "File %s IN_MOVED_TO.\n", event->name);
                        std::string fileName = event->name;
                        const  char *ext = strrchr(fileName.c_str(), '.');
                        string sext(ext+1);
                        if (sext.compare("jpg") == 0 || sext.compare("JPG") == 0)
                            ((NemoFaceRecogWidget*)args)->fileQueue.push(fileName);
                    }
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }
    /*removing the “/tmp” directory from the watch list.*/
    inotify_rm_watch(fd, wd);

    /*closing the INOTIFY instance*/
    ::close(fd);


}

void NemoFaceRecogWidget::showNemoFace(QString name, QImage image)
{
    //QTableWidgetItem()
    qDebug() << "showNemoFace" ;

    //QLabel imgLabel;
    //QIcon* icon = new QIcon(QPixmap::fromImage(image, Qt::AutoColor));
    //imgLabel.setPixmap(QPixmap::fromImage(image, Qt::AutoColor));
    //setItem(1, 1, new QTableWidgetItem(*icon, ""));
//   // setCellWidget(1, 1, &imgLabel);
//    repaint();
   // item(0,0)->setData(Qt::DecorationRole, QPixmap::fromImage(image, Qt::AutoColor));
    // item(0,0)->setText("aaaaaa");
    setItem(0,0,new QTableWidgetItem("Jan"));
    //delete icon;
    return;
    nemoFaceWidgetIndex = (nemoFaceWidgetIndex+1) % 4;
    if (nemoFaceWidget[nemoFaceWidgetIndex] == nullptr)
    {
        nemoFaceWidget[nemoFaceWidgetIndex] = new FaceViewWidget(this, 0, name, image);
        nemoFaceWidget[nemoFaceWidgetIndex]->show();
    }
    else
    {
        nemoFaceWidget[nemoFaceWidgetIndex]->SetNewFace(0, name, image);
    }
}

void NemoFaceRecogWidget::ShowFaceRecogResult(QImage img, int faceIndex, float score)
{

    this->removeRow(2);
    this->insertRow(0);

    if (0)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(Qt::DecorationRole, QPixmap::fromImage(img).scaled(256,256, Qt::KeepAspectRatio));
        setItem(0, 0, item);
        this->item(0, 0)->setTextAlignment(Qt::AlignCenter);
    }
    else if (1)
    {
        QLabel *nemoFaceLabel = new QLabel;
        nemoFaceLabel->setPixmap(QPixmap::fromImage(img, Qt::AutoColor).scaled(256,256, Qt::KeepAspectRatio));
        nemoFaceLabel->setAlignment(Qt::AlignCenter);
        setCellWidget(0, 0, nemoFaceLabel);

        QTableWidgetItem *itemScore = new QTableWidgetItem(QString("%1 %2").arg(score * 100).arg("%"));
        itemScore->setTextAlignment(Qt::AlignCenter);
        QFont font;
        font.setPointSize(20);
        itemScore->setFont(font);
        setItem(0, 1, itemScore);

        QString faceName = mArcFaceEngine->getRegFaceNameByIndex(faceIndex);
        QString fileNameLow = "./faceDB/" + faceName + ".jpg";
        QString fileNameUpper = "./faceDB/" + faceName + ".JPG";
        QImage dbFaceImg;
        bool loadFileSuccess = true;
        if(!(dbFaceImg.load(fileNameLow))) //加载图像
        {
            if (!(dbFaceImg.load(fileNameUpper)))
            {
                qDebug() << "open file failed: " << fileNameLow;
                loadFileSuccess = false;
            }
        }
        if (loadFileSuccess)
        {
            QLabel *dbFaceLabel = new QLabel;
            dbFaceLabel->setPixmap(QPixmap::fromImage(dbFaceImg, Qt::AutoColor).scaled(256,256, Qt::KeepAspectRatio));
            dbFaceLabel->setAlignment(Qt::AlignCenter);
            setCellWidget(0, 2, dbFaceLabel);
        }


        QTableWidgetItem *itemName = new QTableWidgetItem(faceName);
        itemName->setTextAlignment(Qt::AlignCenter);
        itemName->setFont(font);
        setItem(0, 3, itemName);
    }
    else
    {
        QIcon *icon = new QIcon(QPixmap::fromImage(img, Qt::AutoColor).scaled(256, 256, Qt::KeepAspectRatio));
        setItem(0, 0, new QTableWidgetItem(*icon, "abc"));
    }
}

void NemoFaceRecogWidget::frThreadProc(void *args)
{
    std::string fileName;

    while (((NemoFaceRecogWidget*)args)->inotifyRunning)
    {

        fileName = "";
        ((NemoFaceRecogWidget*)args)->fileQueue.try_pop(fileName);
        if (!fileName.empty())
        {

           // fprintf(stderr, fileName.c_str());
            fileName = ((NemoFaceRecogWidget*)args)->faceImageDir + "/" + fileName;
            QImage img;
            if(!(img.load(fileName.c_str()))) //加载图像
            {
                qDebug() << "open file failed: " << fileName.c_str();
                continue;
            }
            img = img.convertToFormat(QImage::Format_RGB888);

            // ((NemoFaceRecogWidget*)args)->showNemoFace("", img);

//            emit(((NemoFaceRecogWidget*)args)->updateFaceRecogResult(img, "aaaaaa", 0.9));
//            continue;

            int frameWidth = img.width();
            int frameHeight = img.height();
            int frameDataSize = frameWidth * frameHeight * 3;
            unsigned char *frameData = new unsigned char[frameDataSize];
            int frameFormat = ASVL_PAF_RGB24_B8G8R8;
            //rgb24_to_yuv420p(frameData, frameData + frameWidth * frameHeight,frameData +  frameWidth * frameHeight * 5 /4, img.bits(), frameWidth, frameHeight);
            rgb24_to_bgr24(img.bits(), frameData, frameWidth, frameHeight);

            int numFace = 0;
            LPAFT_FSDK_FACERES pFaceResult;

            int ret = ((NemoFaceRecogWidget*)args)->mArcFaceEngine->detectFace(frameData, frameWidth, frameHeight, frameFormat, &numFace, &pFaceResult);

            if (numFace <= 0)
            {
                fprintf(stderr, "???no face detected, %s. return: %d\n", fileName.c_str(), ret);
                continue;
            }

            fprintf(stderr, "!!!face detected, %s.\n", fileName.c_str());

            int faceIndex = -1;
            float fscore = 0;
            ret = ((NemoFaceRecogWidget*)args)->mArcFaceEngine->recognitionFace(frameData, frameWidth, frameHeight, frameFormat, &pFaceResult->rcFace[0], pFaceResult->lfaceOrient, &faceIndex, &fscore);


            fprintf(stderr, "face recognize, id: %d, score: %f\n", faceIndex, fscore);


            if (faceIndex >= 0)
                 emit(((NemoFaceRecogWidget*)args)->updateFaceRecogResult(img, faceIndex, fscore));
//               ((NemoFaceRecogWidget*)args)->showNemoFace(((NemoFaceRecogWidget*)args)->mArcFaceEngine->getRegFaceName(faceID), img);

        }
        else
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

}
