#-------------------------------------------------
#
# Project created by QtCreator 2015-04-18T11:16:52
#
#-------------------------------------------------
QT_VERSION = 5

ROOT_DIRECTORY = ../../al_build
LIB_DIRECTORY = $${ROOT_DIRECTORY}/lib
BUILD_DIRECTORY = $${ROOT_DIRECTORY}/build
TESTS_DIRECTORY = $${BUILD_DIRECTORY}/tests
EXAMPLES_DIRECTORY = $${BUILD_DIRECTORY}/examples
EXEC_DIRECTORY = $${BUILD_DIRECTORY}

DESTDIR = $${EXEC_DIRECTORY}
OBJECTS_DIR = $${BUILD_DIRECTORY}
MOC_DIR = $${BUILD_DIRECTORY}
RCC_DIR = $${BUILD_DIRECTORY}

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AlteXo
TEMPLATE = app


#include(../al-ext-libs.pri)

macx {
    include(mac-deps.pri)
}

!macx {
    include(unix-deps.pri)
}


INCLUDEPATH += ../live_streamer
LIBS += -L$$ROOT_DIRECTORY/streamer/build -lal-live-streamer

SOURCES += main.cpp\
        mainwindow.cpp \
    alfreenectdevice.cpp \
    alkinectinterface.cpp \
    alcolor.cpp \
    alrecorder.cpp \
    alvideoappsrc.cpp \
    alvideosurface.cpp \
    settingsdialog.cpp \
    extra/ofxKinectExtras.cpp \
    broadcast/alsharedmemorybroadcaster.cpp

HEADERS  += mainwindow.h \
    alfreenectdevice.h \
    alkinectinterface.h \
    alcolor.h \
    alrecorder.h \
    alvideoappsrc.h \
    alvideosurface.h \
    settingsdialog.h \
    extra/ofxKinectExtras.h \
    broadcast/alsharedmemorybroadcaster.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

