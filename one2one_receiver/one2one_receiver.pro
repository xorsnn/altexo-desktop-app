QT_VERSION = 5

QT += core gui

ROOT_DIRECTORY = ../../al_build/streamer2
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

TARGET = al-one2one-receiver
TEMPLATE = lib
CONFIG += plugin

DEFINES += POSIX
DEFINES += LINUX
DEFINES += WEBRTC_POSIX

CONFIG += c++11
CONFIG += no_keywords

unix: CONFIG += link_pkgconfig

include(../al-ext-libs.pri)

INCLUDEPATH+=../AlteXo/interfaces/

QMAKE_CXXFLAGS += -fno-rtti
QMAKE_CXXFLAGS += -fPIC

HEADERS += \
    conductor.h \
    alstreamer.h \
    streamerplugin.h \
    customsocketserver.h \
    defaults.h \
    alconductor.h \
    alvideocapturer.h \
    alvideocapturerthread.h

SOURCES += \
    conductor.cc \
    alstreamer.cpp \
    streamerplugin.cpp \
    defaults.cc \
    alconductor.cpp \
    alvideocapturer.cpp \
    alvideocapturerthread.cpp
