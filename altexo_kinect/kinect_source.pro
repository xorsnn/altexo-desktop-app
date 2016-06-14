#-------------------------------------------------
#
# Project created by QtCreator 2016-02-24T21:22:24
#
#-------------------------------------------------

QT_VERSION = 5

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

ROOT_DIRECTORY = ../kinect_source_build/source
LIB_DIRECTORY = $${ROOT_DIRECTORY}/lib
BUILD_DIRECTORY = $${ROOT_DIRECTORY}/build
TESTS_DIRECTORY = $${BUILD_DIRECTORY}/tests
EXAMPLES_DIRECTORY = $${BUILD_DIRECTORY}/examples
EXEC_DIRECTORY = $${BUILD_DIRECTORY}

DESTDIR = $${EXEC_DIRECTORY}
OBJECTS_DIR = $${BUILD_DIRECTORY}
MOC_DIR = $${BUILD_DIRECTORY}
RCC_DIR = $${BUILD_DIRECTORY}

QMAKE_CXXFLAGS += -fPIC

macx {
    include(mac-deps.pri)
}

!macx {
    include(unix-deps.pri)
}

INCLUDEPATH += ../al_sdk_tester/interfaces/

TARGET = kinect_source
TEMPLATE = lib

DEFINES += KINECT_SOURCE_LIBRARY

SOURCES += \
    alfreenectdevice.cpp \
    alkinectinterface.cpp \
    alcolor.cpp \
    sensorplugin.cpp

HEADERS += \
    alfreenectdevice.h \
    alkinectinterface.h \
    alcolor.h \
    sensorplugin.h

