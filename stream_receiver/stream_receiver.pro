QT_VERSION = 5

QT += core qml quick core-private gui-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stream_receiver
TEMPLATE = app


#DEFINES += POSIX
#DEFINES += LINUX
#DEFINES += WEBRTC_POSIX

#CONFIG += c++11
#CONFIG += no_keywords

#unix: CONFIG += link_pkgconfig

#include(../al-ext-libs.pri)

INCLUDEPATH+=../AlteXo/interfaces/

SOURCES += main.cpp \
    alvideorenderer.cpp \
    threadrenderer.cpp

HEADERS  += \
    alvideorenderer.h \
    threadrenderer.h

FORMS    +=

DISTFILES += \
    main.qml

RESOURCES += \
    mail.qrc
