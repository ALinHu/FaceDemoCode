#-------------------------------------------------
#
# Project created by QtCreator 2017-11-30T11:51:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = NemoFaceDemo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += inc
LIBS        +=  -L../NemoFaceDemo/lib/linux_x64 -larcsoft_fsdk_face_detection -larcsoft_fsdk_face_tracking -larcsoft_fsdk_face_recognition -ljpeg

SOURCES += \
            main.cpp \
        mainwindow.cpp \
    arcfaceengine.cpp \
    camerasource.cpp \
    faceviewwidget.cpp \
    glhelper.cpp \
    rgb2yuv.cpp \
    videofilesource.cpp \
    cameravideowidget.cpp \
    nemofacerecogwidget.cpp \
    jpegfile.cpp \
    facestillimageview.cpp

HEADERS += \
        mainwindow.h \
    arcfaceengine.h \
    camerasource.h \
    faceviewwidget.h \
    glhelper.h \
    key.h \
    rgb2yuv.h \
    SafeQueue.hpp \
    videofilesource.h \
    framesource.h \
    cameravideowidget.h \
    nemofacerecogwidget.h \
    jpegfile.h \
    facestillimageview.h

FORMS += \
        mainwindow.ui
