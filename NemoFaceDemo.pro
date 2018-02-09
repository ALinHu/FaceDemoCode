#-------------------------------------------------
#
# Project created by QtCreator 2017-11-30T11:51:08
#
#-------------------------------------------------

QT       += core gui
#FileAdd
QT += core
QT += gui
QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = NemoFaceDemo
TEMPLATE = app
CONFIG += C++11
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
LIBS        +=  -L/home/deepglint/Documents/qtCode/DoorFace/NemoFace/lib/linux_x64 -larcsoft_fsdk_face_detection -larcsoft_fsdk_face_tracking -larcsoft_fsdk_face_recognition -ljpeg

SOURCES += \
            main.cpp \
        mainwindow.cpp \
    arcfaceengine.cpp \
    faceviewwidget.cpp \
    rgb2yuv.cpp \
    nemofacerecogwidget.cpp

HEADERS += \
        mainwindow.h \
    arcfaceengine.h \
    faceviewwidget.h \
    key.h \
    rgb2yuv.h \
    SafeQueue.hpp \
    nemofacerecogwidget.h

FORMS += \
        mainwindow.ui
