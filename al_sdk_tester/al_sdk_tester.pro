#-------------------------------------------------
#
# Project created by QtCreator 2016-04-30T19:36:38
#
#-------------------------------------------------

##
# CONFIG
#
WEBRTCTRUNK = /home/xors/workspace/ALTEXO/webrtc/src

INCLUDEPATH += .
INCLUDEPATH += $$WEBRTCTRUNK/
INCLUDEPATH += $$WEBRTCTRUNK/chromium/src/third_party/jsoncpp/source/include/
INCLUDEPATH += $$WEBRTCTRUNK/third_party/
INCLUDEPATH += $$WEBRTCTRUNK/third_party/webrtc/
INCLUDEPATH += $$WEBRTCTRUNK/webrtc/
INCLUDEPATH += $$WEBRTCTRUNK/net/third_party/nss/ssl
INCLUDEPATH += $$WEBRTCTRUNK/third_party/jsoncpp/overrides/include
INCLUDEPATH += $$WEBRTCTRUNK/third_party/jsoncpp/source/include

INCLUDEPATH += /usr/include/nss
INCLUDEPATH += /usr/include/nspr

INCLUDEPATH += ../altexo_kinect/

DEFINES += POSIX

#unix: {
  DEFINES += LINUX
#}
#unix: {
  DEFINES += WEBRTC_POSIX
#}
#win32: {
#  DEFINES += WEBRTC_WIN
#}

CONFIG += c++11
CONFIG += no_keywords

#unix: {
#    include(al-ext-libs.pri)
#}
# ===============================

INCLUDEPATH += /home/xors/workspace/QT/ALTEXO_SDK

#LIBS += -L/home/xors/workspace/QT/build-al_manager-dev-Release -lal_manager
LIBS += -lm -ldl

QMAKE_CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=0

QT += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = al_sdk_tester
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    peerclientcallback.cpp \
    alconnclient.cpp

HEADERS  += mainwindow.h \
    peerclientcallback.h \
    alconnclient.h \
    interfaces/AlSensorInterface.h

FORMS    += mainwindow.ui
