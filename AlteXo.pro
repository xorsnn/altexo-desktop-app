#-------------------------------------------------
#
# Project created by QtCreator 2015-04-18T11:16:52
#
#-------------------------------------------------
QT_VERSION = 5

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AlteXo
TEMPLATE = app

!macx {
    CONFIG += link_pkgconfig
#        compile_libtool

    PKGCONFIG += gstreamer-1.0 libusb-1.0 gstreamer-audio-1.0 gstreamer-video-1.0 gstreamer-pbutils-1.0 gobject-2.0
}

macx {
    INCLUDEPATH += /opt/local/include/gstreamer-1.0
    INCLUDEPATH += /opt/local/include/glib-2.0
    INCLUDEPATH += /opt/local/lib/glib-2.0/include/
    INCLUDEPATH += /usr/local/Cellar/libfreenect/0.5.1/include/libfreenect
    INCLUDEPATH += /usr/local/Cellar/libusb/1.0.19/include/libusb-1.0/
    INCLUDEPATH += /Users/alexpasch/workspace/LIBS/QT_GSTREAMER/include/Qt5GStreamer
    INCLUDEPATH += /opt/local/include

    LIBS += -L/opt/local/lib/ -lgstreamer-1.0
    LIBS += -L/opt/local/lib -lgobject-2.0
    LIBS += -L/usr/local/lib -lfreenect
    LIBS += -L/usr/local/lib -lglut


    LIBS += -L/Users/alexpasch/workspace/LIBS/QT_GSTREAMER/lib -lQt5GStreamer-1.0
    LIBS += -L/Users/alexpasch/workspace/LIBS/QT_GSTREAMER/lib -lQt5GStreamerUtils-1.0
    LIBS += -L/Users/alexpasch/workspace/LIBS/QT_GSTREAMER/lib -lQt5GLib-2.0

    LIBS += -pthread \
            -lgmodule-2.0 \
            -lgthread-2.0 \
            #-lrt \
            -lglib-2.0

            #-lGL \

#            -lfreenect \
#            -lGLU
}

!macx {
    INCLUDEPATH += /usr/include/gstreamer-1.0
    INCLUDEPATH += /usr/include/glib-2.0
    INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include/
    INCLUDEPATH += /usr/include/libfreenect/
    INCLUDEPATH += /usr/include/Qt5GStreamer

    LIBS += -pthread -lgstreamer-1.0 -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lglib-2.0 \
        -lrt \
        -lGL \
        -lglut \
        -lfreenect \
        -lGLU \
        -lQt5GLib-2.0 \
        -lQt5GStreamer-1.0 \
        -lQt5GStreamerUtils-1.0


#    LIBS += -L/home/xors/workspace/LIBS/INSTALL/lib -lQt5GLib-2.0
#    LIBS += -L/home/xors/workspace/LIBS/INSTALL/lib -lQt5GStreamer-1.0
#    LIBS += -L/home/xors/workspace/LIBS/INSTALL/lib -lQt5GStreamerUtils-1.0
}

SOURCES += main.cpp\
        mainwindow.cpp \
    alfreenectdevice.cpp \
    alkinectinterface.cpp \
    alcolor.cpp \
    alrecorder.cpp \
    alvideoappsrc.cpp \
    alvideosurface.cpp \
    settingsdialog.cpp \
    extra/ofxKinectExtras.cpp \
    broadcast/alsharedmemorybroadcaster.cpp

HEADERS  += mainwindow.h \
    alfreenectdevice.h \
    alkinectinterface.h \
    alcolor.h \
    alrecorder.h \
    alvideoappsrc.h \
    alvideosurface.h \
    settingsdialog.h \
    extra/ofxKinectExtras.h \
    broadcast/alsharedmemorybroadcaster.h

FORMS    += mainwindow.ui \
    settingsdialog.ui
