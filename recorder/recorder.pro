
QT_VERSION = 5

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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

