#-------------------------------------------------
#
# Project created by QtCreator 2015-04-18T11:16:52
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AlteXo
TEMPLATE = app

!macx {
    CONFIG += link_pkgconfig compile_libtool
}

CONFIG -= silent

macx {
    INCLUDEPATH += /opt/local/include/gstreamer-1.0
    INCLUDEPATH += /opt/local/include/glib-2.0
    INCLUDEPATH += /opt/local/lib/glib-2.0/include/
    INCLUDEPATH += /usr/local/Cellar/libfreenect/0.5.1/include/libfreenect
    INCLUDEPATH += /usr/local/Cellar/libusb/1.0.19/include/libusb-1.0/

    #LIBS += -L/opt/local/lib/gstreamer-1.0
    LIBS += -L/opt/local/lib/ -lgstreamer-1.0
    LIBS += -L/opt/local/lib -lgobject-2.0
    LIBS += -L/usr/local/lib -lfreenect
    LIBS += -L/usr/local/lib -lglut
    LIBS += -pthread \
            -lgmodule-2.0 \
            -lgthread-2.0 \
            #-lrt \
            -lglib-2.0 \
            #-lGL \

#            -lfreenect \
#            -lGLU
}

!macx {
    INCLUDEPATH += /usr/include/gstreamer-1.0
    INCLUDEPATH += /usr/include/glib-2.0
    INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include/
    INCLUDEPATH += /usr/include/libfreenect/

    LIBS += -pthread -lgstreamer-1.0 -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lglib-2.0 \
        -lGL -lglut -lfreenect -lGLU
}

#-lgstgl-1.0 # -lgstvideo-1.0  \-lGLEW

#LIBS += -pthread -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include


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
CONFIG -= silent
