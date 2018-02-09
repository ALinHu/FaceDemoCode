TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    genFaceFeature.cpp \
    jpegfile.cpp
INCLUDEPATH += inc
LIBS        +=  -L/home/deepglint/Documents/qtCode/DoorFace/NemoFace/lib/linux_x64 -larcsoft_fsdk_face_detection -larcsoft_fsdk_face_tracking -larcsoft_fsdk_face_recognition -ljpeg

HEADERS += \
    jpegfile.h
