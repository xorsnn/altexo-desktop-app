#-------------------------------------------------
#
# Project created by QtCreator 2016-04-26T19:00:26
#
#-------------------------------------------------

TARGET = al_manager

TEMPLATE = lib

DEFINES += POSIX

unix: {
  DEFINES += LINUX
}
unix: {
  DEFINES += WEBRTC_POSIX
}
win32: {
  DEFINES += WEBRTC_WIN
}

CONFIG += c++11
CONFIG += no_keywords

#unix: CONFIG += link_pkgconfig

unix: {
    include(al-ext-libs.pri)
}

win32: {
  include(al-ext-libs-win.pri)
}

QMAKE_CXXFLAGS += -fno-rtti
#QMAKE_CXXFLAGS += -fPIC

QMAKE_CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=0

#DEFINES += AL_KONDUCTOR_LIBRARY


LIBS += -lboost_thread -lboost_system

SOURCES += \
    almanager.cpp \
    conductor/conductor.cc \
    conductor/defaults.cc \
    conductor/peer_connection_client.cc \
    alvideorenderer.cpp \
    conductor/aldatamanager.cpp \
    almanagerinterface.cpp \
    alvideocapturer.cpp \
    alvideocapturerthread.cpp

HEADERS += \
    almanager.h \
    conductor/conductor.h \
    customsocketserver.h \
    conductor/defaults.h \
    conductor/peer_connection_client.h \
    alcallback.h \
    alvideorenderer.h \
    conductor/aldatamanager.h \
    almanagerinterface.h \
    alvideocapturer.h \
    alvideocapturerthread.h

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}
