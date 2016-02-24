INCLUDEPATH += /opt/local/include/gstreamer-1.0
INCLUDEPATH += /opt/local/include/glib-2.0
INCLUDEPATH += /opt/local/lib/glib-2.0/include/
INCLUDEPATH += /usr/local/Cellar/libusb/1.0.19/include/libusb-1.0/
INCLUDEPATH += /Users/alexpasch/workspace/LIBS/QT_GSTREAMER/include/Qt5GStreamer
INCLUDEPATH += /opt/local/include

LIBS += -L/opt/local/lib/ -lgstreamer-1.0
LIBS += -L/opt/local/lib -lgobject-2.0
LIBS += -L/usr/local/lib -lglut

LIBS += -L/Users/alexpasch/workspace/LIBS/QT_GSTREAMER/lib -lQt5GStreamer-1.0
LIBS += -L/Users/alexpasch/workspace/LIBS/QT_GSTREAMER/lib -lQt5GStreamerUtils-1.0
LIBS += -L/Users/alexpasch/workspace/LIBS/QT_GSTREAMER/lib -lQt5GLib-2.0

LIBS += -pthread \
        -lgmodule-2.0 \
        -lgthread-2.0 \
        -lglib-2.0

