#-------------------------------------------------
#
# Project created by QtCreator 2016-02-28T18:39:29
#
#-------------------------------------------------

QT       += qml quick core-private gui-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stream_receiver
TEMPLATE = app


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
