#-------------------------------------------------
#
# Project created by QtCreator 2014-02-17T17:01:31
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_IPCamStreaming
TEMPLATE = app

###############################################################

LIBS += opengl32.lib
#LIBS += qwindows.lib
LIBS += Qt5PlatformSupport.lib

LIBS += $$PWD/Decoder/ffmpeg/bin/avcodec.lib
LIBS += $$PWD/Decoder/ffmpeg/bin/avformat.lib
LIBS += $$PWD/Decoder/ffmpeg/bin/avutil.lib
LIBS += $$PWD/Decoder/ffmpeg/bin/swscale.lib

LIBS += $$PWD/RTSPCLIENT/VideoSource.lib

###############################################################

SOURCES += main.cpp\
        mainwindow.cpp \
    Decoder/AVS_AvDecoder.cpp \
    Painting/CVDGraphicsItems.cpp \
    Painting/CVDGraphScene.cpp \
    Painting/CVDGraphView.cpp \
    RTSPCLIENT/CamSet.cpp \
    RTSPCLIENT/GlobalApp.cpp \
    RTSPCLIENT/RtspClient.cpp \
    DISWindow.cpp

HEADERS  += mainwindow.h \
            PeopleCountLib.h \
    Decoder/AVS_AvDecoder.h \
    Painting/CVDGraphicsItems.h \
    Painting/CVDGraphScene.h \
    Painting/CVDGraphView.h \
    Painting/CVDParameters.h \
    RTSPCLIENT/CamSet.h \
    RTSPCLIENT/GlobalApp.h \
    RTSPCLIENT/RtspClient.h \
    RTSPCLIENT/VideoSource.h \
    RTSPCLIENT/VideoTypes.h \
    DISWindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/


INCLUDEPATH += $$PWD/Decoder/ffmpeg/inc/
INCLUDEPATH += $$PWD/Decoder/
INCLUDEPATH += $$PWD/RTSPCLIENT/
INCLUDEPATH += $$PWD/Painting/


