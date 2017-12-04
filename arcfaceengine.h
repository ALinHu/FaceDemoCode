#ifndef ARCFACEENGINE_H
#define ARCFACEENGINE_H

#include <QObject>
#include <QVector>

#include "asvloffscreen.h"
#include "arcsoft_fsdk_face_detection.h"
#include "arcsoft_fsdk_face_recognition.h"
#include "arcsoft_fsdk_face_tracking.h"

#include <vector>
using namespace std;


#define MAX_FD_FACE (50)
#define MAX_FT_FACE (50)

class FaceInfo{
public:
    unsigned char *pFeature;
    int featureSize;
    QString name;
    int id;
    FaceInfo(){
        pFeature = NULL;
        id = -1;
    }
 };


class ArcFaceEngine : public QObject
{
    Q_OBJECT
public:
    explicit ArcFaceEngine();
    virtual ~ArcFaceEngine();

    // load face form faceDB dir
    int loadFaceDB();


    bool processFrame(unsigned char *frameData,int frameWidth,int frameHeight,int frameFormat);
    void removeFace(int id);
    void updateFaceName(int id,QString name);
    int registerFace(unsigned char *frameData,int frameWidth,int frameHeight,int frameFormat,MRECT *pRect,int faceOrient,int *pID);
    int recognitionFace(unsigned char *frameData,int frameWidth,int frameHeight,int frameFormat,MRECT *pRect,int faceOrient,int *pID,float *pfScore);
    int detectFace(unsigned char *frameData, int frameWidth, int frameHeight, int frameFormat, int *faceCount, LPAFT_FSDK_FACERES *ppFaceResult);
    int detectFace_FD(unsigned char *frameData, int frameWidth, int frameHeight, int frameFormat, int &faceCount, LPAFD_FSDK_FACERES &pFaceResult);


    MRECT mFaceRect[MAX_FT_FACE];
    int mFaceOrient[MAX_FT_FACE];
    QString mFaceName[MAX_FT_FACE];
    int mFaceID[MAX_FT_FACE];
    int mFaceNum;
    int mUniqueIncID;

    QString getRegFaceName(int id) {
        QVector<FaceInfo>::iterator iter;
        for (iter=mRegisterFaces.begin();iter!=mRegisterFaces.end();iter++)
        {
            if (iter->id == id)
                return iter->name;
        }
        return "";
    }

    QString getRegFaceNameByIndex(int index) {
        return mRegisterFaces[index].name;
    }

public slots:

private:
    void *hFDEngine;
    unsigned char *mFDWorkMem;

    void *hFTEngine;
    unsigned char *mFTWorkMem;

    void *hFREngine;
    unsigned char *mFRWorkMem;

    QVector<FaceInfo> mRegisterFaces;

    float mThreshold;

    bool bReady;
    int doFRFrameCount;

    QString getFaceNameByID(int id);
    int ExtractFRFeature(unsigned char *frameData,int frameWidth,int frameHeight,int frameFormat,MRECT *pRect,int faceOrient,AFR_FSDK_FACEMODEL *pFaceModels);

    int getAllFile(string dir, string ext, vector<string> &files);
};

#endif // ARCFACEENGINE_H
