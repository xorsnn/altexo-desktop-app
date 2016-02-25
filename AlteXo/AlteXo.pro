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

QT += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AlteXo
TEMPLATE = app

#DEFINES += POSIX
#DEFINES += LINUX
#DEFINES += WEBRTC_POSIX

#CONFIG += c++11
#CONFIG += no_keywords


macx {
    include(mac-deps.pri)
}

!macx {
    include(unix-deps.pri)
}


SOURCES += main.cpp\
        mainwindow.cpp \
    alvideosurface.cpp \
    settingsdialog.cpp \
    alwsclient.cpp

HEADERS  += mainwindow.h \
    alvideosurface.h \
    settingsdialog.h \
    interfaces/AlStreamerInterface.h \
    alwsclient.h \
    extra/alstreamerconnector.h \
    interfaces/AlRecorderInterface.h \
    interfaces/AlSensorInterface.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

