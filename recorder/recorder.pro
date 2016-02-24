
QT_VERSION = 5

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

ROOT_DIRECTORY = ../../al_build/recorder
LIB_DIRECTORY = $${ROOT_DIRECTORY}/lib
BUILD_DIRECTORY = $${ROOT_DIRECTORY}/build
TESTS_DIRECTORY = $${BUILD_DIRECTORY}/tests
EXAMPLES_DIRECTORY = $${BUILD_DIRECTORY}/examples
EXEC_DIRECTORY = $${BUILD_DIRECTORY}

DESTDIR = $${EXEC_DIRECTORY}
OBJECTS_DIR = $${BUILD_DIRECTORY}
MOC_DIR = $${BUILD_DIRECTORY}
RCC_DIR = $${BUILD_DIRECTORY}

macx {
    include(mac-deps.pri)
}

!macx {
    include(unix-deps.pri)
}

INCLUDEPATH += ../AlteXo/interfaces/

TARGET = recorder
TEMPLATE = lib

DEFINES += RECORDER_LIBRARY

SOURCES += \
    alrecorder.cpp \
    alvideoappsrc.cpp \
    recorderplugin.cpp

HEADERS += \
    alrecorder.h \
    alvideoappsrc.h \
    recorderplugin.h

