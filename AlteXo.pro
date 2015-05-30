#-------------------------------------------------
#
# Project created by QtCreator 2015-04-18T11:16:52
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AlteXo
TEMPLATE = app

CONFIG += link_pkgconfig compile_libtool

INCLUDEPATH += /usr/include/gstreamer-1.0
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include/
INCLUDEPATH += /usr/include/libfreenect/

PKGCONFIG = gstreamer-1.0 libusb-1.0

LIBS += -pthread -lgstreamer-1.0 -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lglib-2.0 \
    -lGL -lglut -lfreenect -lGLU

#-lgstgl-1.0 # -lgstvideo-1.0  \-lGLEW

LIBS += -pthread -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include


SOURCES += main.cpp\
        mainwindow.cpp \
    recorder.cpp \
    alglwidget.cpp \
    alfreenectdevice.cpp \
    alkinectinterface.cpp \
    alcolor.cpp

HEADERS  += mainwindow.h \
    recorder.h \
    alglwidget.h \
    alfreenectdevice.h \
    alkinectinterface.h \
    alcolor.h

FORMS    += mainwindow.ui
