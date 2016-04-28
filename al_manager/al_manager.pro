#-------------------------------------------------
#
# Project created by QtCreator 2016-04-26T19:00:26
#
#-------------------------------------------------

TARGET = al_manager

TEMPLATE = lib

DEFINES += POSIX
DEFINES += LINUX
DEFINES += WEBRTC_POSIX

CONFIG += c++11
CONFIG += no_keywords

unix: CONFIG += link_pkgconfig

include(../al-ext-libs.pri)

QMAKE_CXXFLAGS += -fno-rtti
QMAKE_CXXFLAGS += -fPIC

DEFINES += AL_KONDUCTOR_LIBRARY

SOURCES += \
    almanager.cpp \
    conductor/conductor.cc \
    conductor/defaults.cc \
    conductor/peer_connection_client.cc

HEADERS += \
    almanager.h \
    conductor/conductor.h \
    customsocketserver.h \
    conductor/defaults.h \
    conductor/peer_connection_client.h \
    alcallback.h

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}
