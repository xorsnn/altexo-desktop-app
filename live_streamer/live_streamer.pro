#-------------------------------------------------
#
# Project created by QtCreator 2016-01-03T18:01:51
#
#-------------------------------------------------
QT_VERSION = 5

QT += core gui

ROOT_DIRECTORY = ../../al_build/streamer
LIB_DIRECTORY = $${ROOT_DIRECTORY}/lib
BUILD_DIRECTORY = $${ROOT_DIRECTORY}/build
TESTS_DIRECTORY = $${BUILD_DIRECTORY}/tests
EXAMPLES_DIRECTORY = $${BUILD_DIRECTORY}/examples
EXEC_DIRECTORY = $${BUILD_DIRECTORY}

DESTDIR = $${EXEC_DIRECTORY}
OBJECTS_DIR = $${BUILD_DIRECTORY}
MOC_DIR = $${BUILD_DIRECTORY}
RCC_DIR = $${BUILD_DIRECTORY}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = al-live-streamer
#TEMPLATE = app
#TEMPLATE = lib
#CONFIG += staticlib
TEMPLATE = lib
CONFIG += plugin

#EXAMPLE_FILES = mymetadata.json

DEFINES += POSIX
DEFINES += LINUX
DEFINES += WEBRTC_POSIX

CONFIG += c++11
CONFIG += no_keywords

unix: CONFIG += link_pkgconfig

include(../al-ext-libs.pri)

INCLUDEPATH+=../AlteXo/interfaces/

QMAKE_CXXFLAGS += -fno-rtti


SOURCES += main.cpp \
    defaults.cc \
    alinterface/almainwindow.cpp \
    alconductor/alconductor.cpp \
    alconductor/alvideocapturer.cpp \
    alinterface/widgetstreamer.cpp \
    alconductor/alvideocapturerthread.cpp \
    alstreamer.cpp \
    streamerplugin.cpp

HEADERS  += \
    defaults.h \
    flagdefs.h \
    alinterface/almainwindow.h \
    alconductor/alconductor.h \
    alconductor/alvideocapturer.h \
    alinterface/widgetstreamer.h \
    alconductor/alvideocapturerthread.h \
    customsocketserver.h \
    alstreamer.h \
    streamerplugin.h

FORMS    += \
    alinterface/almainwindow.ui
