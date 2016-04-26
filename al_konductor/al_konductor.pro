#-------------------------------------------------
#
# Project created by QtCreator 2016-04-26T19:00:26
#
#-------------------------------------------------

QT       -= core gui

TARGET = al_konductor
TEMPLATE = lib

DEFINES += AL_KONDUCTOR_LIBRARY

SOURCES += alconductor.cpp

HEADERS += alconductor.h\
        al_konductor_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
