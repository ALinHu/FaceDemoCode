#include <dirent.h> //遍历系统指定目录下文件要包含的头文件
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string.h>
#include "arcsoft_fsdk_face_detection.h"
#include "arcsoft_fsdk_face_recognition.h"
#include "merror.h"
#include "jpegfile.h"

using namespace std;

#define APPID "E4Af86vZxS2othNTgbq6wpqYsUfdfY4iNgP3XfpJQir1"
#define FD_SDKKEY "HAB2gckspPzLPf5anoroqyo8mrLBxrUHqAJSCH1y4H84"
#define FR_SDKKEY "HAB2gckspPzLPf5anoroqyodRTNu2qQcCW4nZCMLZ8uc"

#define WORKBUF_SIZE (40*1024*1024)
#define MAX_FACE_NUM (50)

MHandle hFDEngine = nullptr;
MHandle hFREngine = nullptr;

#define IMAGE_BUFFER_SIZE (1920*1080*3)

int getAllFile(string dir, string ext, vector<string> &files)
{
    DIR *pDir;
    if (dir.empty())
        pDir = opendir(".");
    else
        pDir = opendir(dir.c_str());

    if (nullptr == pDir)
    {
        cout << "Can not open dir: " << dir << endl;
        return -1;
    }

    dirent *pEnt = NULL;
    string ext_low = "";
    if (!ext.empty())
    {
        transform(ext.begin(), ext.end(), back_inserter(ext_low), ::tolower);
    }
    while ((pEnt = readdir(pDir)) != nullptr)
    {
        if (strcmp(pEnt->d_name, ".") == 0 || strcmp(pEnt->d_name, "..") == 0)
            continue;

        if (!ext.empty())
        {
            char *ext_file = strrchr(pEnt->d_name, '.');
            if (ext_file == NULL || *(ext_file + 1) == 0)
                continue;

            string tmp = string(ext_file + 1);
            string ext_file_low = "";
            transform(tmp.begin(), tmp.end(), back_inserter(ext_file_low), ::tolower);
            if (ext_file_low.compare(ext_low) == 0)
                files.push_back(string(pEnt->d_name));
        }
        else
            files.push_back(string(pEnt->d_name));
    }
    closedir(pDir);

    return 0;
}

int detectFace(unsigned char *frameData, int frameWidth, int frameHeight, int frameFormat, int &faceCount, LPAFD_FSDK_FACERES &pFaceResult)
{
    int imgSize = 0;
    ASVLOFFSCREEN inputImg = {0};
    inputImg.u32PixelArrayFormat = frameFormat;
    inputImg.i32Width = frameWidth;
    inputImg.i32Height = frameHeight;
    inputImg.ppu8Plane[0] = frameData;
    if (ASVL_PAF_I420 == inputImg.u32PixelArrayFormat)
    {
        inputImg.pi32Pitch[0] = inputImg.i32Width;
        inputImg.pi32Pitch[1] = inputImg.i32Width / 2;
        inputImg.pi32Pitch[2] = inputImg.i32Width / 2;
        inputImg.ppu8Plane[1] = inputImg.ppu8Plane[0] + inputImg.pi32Pitch[0] * inputImg.i32Height;
        inputImg.ppu8Plane[2] = inputImg.ppu8Plane[1] + inputImg.pi32Pitch[1] * inputImg.i32Height / 2;
        imgSize = inputImg.i32Width * inputImg.i32Height * 3 / 2;
    }
    else if (ASVL_PAF_NV12 == inputImg.u32PixelArrayFormat)
    {
        inputImg.pi32Pitch[0] = inputImg.i32Width;
        inputImg.pi32Pitch[1] = inputImg.i32Width;
        inputImg.ppu8Plane[1] = inputImg.ppu8Plane[0] + (inputImg.pi32Pitch[0] * inputImg.i32Height);
        imgSize = inputImg.i32Width * inputImg.i32Height * 3 / 2;
    }
    else if (ASVL_PAF_NV21 == inputImg.u32PixelArrayFormat)
    {
        inputImg.pi32Pitch[0] = inputImg.i32Width;
        inputImg.pi32Pitch[1] = inputImg.i32Width;
        inputImg.ppu8Plane[1] = inputImg.ppu8Plane[0] + (inputImg.pi32Pitch[0] * inputImg.i32Height);
        imgSize = inputImg.i32Width * inputImg.i32Height * 3 / 2;
    }
    else if (ASVL_PAF_YUYV == inputImg.u32PixelArrayFormat)
    {
        inputImg.pi32Pitch[0] = inputImg.i32Width * 2;
        imgSize = inputImg.i32Width * inputImg.i32Height * 2;
    }
    else if (ASVL_PAF_RGB24_B8G8R8 == inputImg.u32PixelArrayFormat)
    {
        inputImg.pi32Pitch[0] = inputImg.i32Width * 3;
        imgSize = inputImg.i32Width * inputImg.i32Height * 3;
    }
    else
    {
        cout << "unsupported Image format:" << inputImg.u32PixelArrayFormat << endl;
        return -1;
    }

    int ret = AFD_FSDK_StillImageFaceDetection(hFDEngine, &inputImg, &pFaceResult);
    if (ret != 0)
    {
        faceCount = 0;
    }
    else
    {
        faceCount = pFaceResult->nFace;
    }
    return 0;
}

int extractFaceFeature(unsigned char *frameData, int frameWidth, int frameHeight, int frameFormat, MRECT faceRect, int faceOrient, AFR_FSDK_FACEMODEL &faceModel)
{
    ASVLOFFSCREEN inputImg = {0};
    inputImg.u32PixelArrayFormat = frameFormat;
    inputImg.i32Width = frameWidth;
    inputImg.i32Height = frameHeight;
    inputImg.ppu8Plane[0] = frameData;
    if (ASVL_PAF_I420 == inputImg.u32PixelArrayFormat)
    {
        inputImg.pi32Pitch[0] = inputImg.i32Width;
        inputImg.pi32Pitch[1] = inputImg.i32Width / 2;
        inputImg.pi32Pitch[2] = inputImg.i32Width / 2;
        inputImg.ppu8Plane[1] = inputImg.ppu8Plane[0] + inputImg.pi32Pitch[0] * inputImg.i32Height;
        inputImg.ppu8Plane[2] = inputImg.ppu8Plane[1] + inputImg.pi32Pitch[1] * inputImg.i32Height / 2;
    }
    else if (ASVL_PAF_NV12 == inputImg.u32PixelArrayFormat)
    {
        inputImg.pi32Pitch[0] = inputImg.i32Width;
        inputImg.pi32Pitch[1] = inputImg.i32Width;
        inputImg.ppu8Plane[1] = inputImg.ppu8Plane[0] + (inputImg.pi32Pitch[0] * inputImg.i32Height);
    }
    else if (ASVL_PAF_NV21 == inputImg.u32PixelArrayFormat)
    {
        inputImg.pi32Pitch[0] = inputImg.i32Width;
        inputImg.pi32Pitch[1] = inputImg.i32Width;
        inputImg.ppu8Plane[1] = inputImg.ppu8Plane[0] + (inputImg.pi32Pitch[0] * inputImg.i32Height);
    }
    else if (ASVL_PAF_YUYV == inputImg.u32PixelArrayFormat)
    {
        inputImg.pi32Pitch[0] = inputImg.i32Width * 2;
    }
    else if (ASVL_PAF_RGB24_B8G8R8 == inputImg.u32PixelArrayFormat)
    {
        inputImg.pi32Pitch[0] = inputImg.i32Width * 3;
    }
    else
    {
        cout << "unsupported Image format:" << inputImg.u32PixelArrayFormat;
        return -1;
    }

    AFR_FSDK_FACEINPUT faceInput;
    faceInput.lOrient = faceOrient;
    faceInput.rcFace.left = faceRect.left;
    faceInput.rcFace.top = faceRect.top;
    faceInput.rcFace.right = faceRect.right;
    faceInput.rcFace.bottom = faceRect.bottom;

    fprintf(stderr, "extrace face feature: (%d, %d, %d, %d)[%d]\n", faceRect.left, faceRect.top, faceRect.right, faceRect.bottom, faceOrient);

    return AFR_FSDK_ExtractFRFeature(hFREngine, &inputImg, &faceInput, &faceModel);
}

int main(int argc, char *argv[])
{
    string faceimgdir = ".";
    if (argc > 1)
        faceimgdir = argv[1];

    MByte *pFDWorkMem = new unsigned char[WORKBUF_SIZE];
    int ret;
    ret = AFD_FSDK_InitialFaceEngine(APPID, FD_SDKKEY, pFDWorkMem, WORKBUF_SIZE, &hFDEngine, AFD_FSDK_OPF_0_HIGHER_EXT, 16, MAX_FACE_NUM);
    if (ret != 0) {
        fprintf(stderr, "fail to AFD_FSDK_InitialFaceEngine(): 0x%x\n", ret);
        delete pFDWorkMem;
        exit(0);
    }

    MByte *pFRWorkMem = new unsigned char[WORKBUF_SIZE];
    ret = AFR_FSDK_InitialEngine(APPID, FR_SDKKEY, pFRWorkMem, WORKBUF_SIZE, &hFREngine);
    if (ret != 0) {
        fprintf(stderr, "fail to AFR_FSDK_InitialEngine(): 0x%x\n", ret);
        free(pFRWorkMem);
        exit(0);
    }

    vector<string> allFiles;

    getAllFile(faceimgdir, "jpg", allFiles);

    unsigned char *rgb_buf = new unsigned char[IMAGE_BUFFER_SIZE];
    int imgWidth = 0;
    int imgHeight = 0;
    int imgPixSize = 0;
    vector<string>::iterator iter;
    for (iter = allFiles.begin(); iter != allFiles.end(); iter++)
    {
        // cout << *iter << endl;
        string filepath = faceimgdir + "/" + (*iter);
        string featurepath = filepath.substr(0, filepath.length()-4) + ".dat";
        cout << featurepath << endl;
        ret = decode_JPEG_file(filepath.c_str(), cs_BGR, &imgWidth, &imgHeight, &imgPixSize, rgb_buf);
        if (ret != 0)
        {
            fprintf(stderr, "fail to decode file: %s\n", (*iter).c_str());
            cout << endl;
            continue;  
        }
        fprintf(stderr, "imgfile: %s. width: %d; height: %d; pix: %d\n", (*iter).c_str(), imgWidth, imgHeight, imgPixSize);
#if 1        
        int faceCount = 0;
        LPAFD_FSDK_FACERES pFaceResult = nullptr;
        ret = detectFace(rgb_buf, imgWidth, imgHeight, ASVL_PAF_RGB24_B8G8R8, faceCount, pFaceResult);
        if (ret != 0 || faceCount == 0)
        {
            fprintf(stderr, "fail to detect face. file: %s\n", (*iter).c_str());
            cout << endl;
            continue;
        }
        fprintf(stderr, "success to detect face. Num: %d, file: %s\n", faceCount, (*iter).c_str());

        AFR_FSDK_FACEMODEL faceModel = { 0 };
        ret = extractFaceFeature(rgb_buf, imgWidth, imgHeight, ASVL_PAF_RGB24_B8G8R8, pFaceResult->rcFace[0], pFaceResult->lfaceOrient[0], faceModel);
        if (ret != 0)
        {
            fprintf(stderr, "fail to extract face feature. file: %s\n", (*iter).c_str());
            cout << endl;
            continue;
        }
        fprintf(stderr, "success to extract face feature. feature size: %d, file: %s\n", faceModel.lFeatureSize, (*iter).c_str());
        ofstream outfile(featurepath, std::ios::binary);
        outfile.write((char*)faceModel.pbFeature, faceModel.lFeatureSize);
        outfile.close();
        // write face feature to file
#endif


        cout << endl;
    }

    AFR_FSDK_UninitialEngine(hFREngine);
    delete pFRWorkMem;
    AFD_FSDK_UninitialFaceEngine(hFDEngine);
    delete pFDWorkMem;

    return 0;
}
