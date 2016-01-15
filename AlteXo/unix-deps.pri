AL_QTGSTREAMER_PATH=/home/xors/PROGS/LIBS/

CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-1.0 libusb-1.0 gstreamer-audio-1.0 gstreamer-video-1.0 gstreamer-pbutils-1.0 gobject-2.0

INCLUDEPATH += /usr/include/gstreamer-1.0
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include/
INCLUDEPATH += /usr/include/libfreenect/
INCLUDEPATH += $$AL_QTGSTREAMER_PATH/include/Qt5GStreamer

LIBS += -pthread -lgstreamer-1.0 -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lglib-2.0 \
    -lrt \
    -lGL \
    -lglut \
    -lfreenect \
    -lGLU

LIBS += -L$$AL_QTGSTREAMER_PATH/lib -lQt5GLib-2.0
LIBS += -L$$AL_QTGSTREAMER_PATH/lib -lQt5GStreamer-1.0
LIBS += -L$$AL_QTGSTREAMER_PATH/lib -lQt5GStreamerUtils-1.0
