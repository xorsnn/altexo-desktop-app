QT_VERSION = 5

QT += core gui websockets qml quick core-private gui-private

ROOT_DIRECTORY = ../../al_build/peer
LIB_DIRECTORY = $${ROOT_DIRECTORY}/lib
BUILD_DIRECTORY = $${ROOT_DIRECTORY}/build
TESTS_DIRECTORY = $${BUILD_DIRECTORY}/tests
EXAMPLES_DIRECTORY = $${BUILD_DIRECTORY}/examplesindow
EXEC_DIRECTORY = $${BUILD_DIRECTORY}

DESTDIR = $${EXEC_DIRECTORY}
OBJECTS_DIR = $${BUILD_DIRECTORY}
MOC_DIR = $${BUILD_DIRECTORY}
RCC_DIR = $${BUILD_DIRECTORY}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += POSIX
DEFINES += LINUX
DEFINES += WEBRTC_POSIX

CONFIG += c++11
CONFIG += no_keywords

unix: CONFIG += link_pkgconfig

include(al-ext-libs.pri)


QMAKE_CXXFLAGS += -fno-rtti
QMAKE_CXXFLAGS += -fno-ident
QMAKE_CXXFLAGS += -fdata-sections
QMAKE_CXXFLAGS += -ffunction-sections
QMAKE_CXXFLAGS += -funwind-tables
QMAKE_CXXFLAGS += -fno-exceptions
QMAKE_CXXFLAGS += -fno-threadsafe-statics
QMAKE_CXXFLAGS += -fvisibility-inlines-hidden

QMAKE_CXXFLAGS += -fstack-protector --param=ssp-buffer-size=4
#QMAKE_CXXFLAGS += -Werror
QMAKE_CXXFLAGS += -pthread
QMAKE_CXXFLAGS += -fno-strict-aliasing
QMAKE_CXXFLAGS += -Wall
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-missing-field-initializers
QMAKE_CXXFLAGS += -fvisibility=hidden
QMAKE_CXXFLAGS += -pipe
QMAKE_CXXFLAGS += -fPIC
#QMAKE_CXXFLAGS += -fcolor-diagnostics


INCLUDEPATH += ../

TARGET = peer_client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    conductor.cc \
    defaults.cc \
    peer_connection_client.cc \
    VideoRenderer.cpp \
    alconnclient.cpp \
    quick_renderer/threadrenderer.cpp \
    quick_renderer/alvideorenderer.cpp

HEADERS  += mainwindow.h \
    conductor.h \
    defaults.h \
    flagdefs.h \
    peer_connection_client.h \
    VideoRenderer.h \
    alconnclient.h \
    quick_renderer/alvideorenderer.h \
    quick_renderer/threadrenderer.h

FORMS    += mainwindow.ui

DISTFILES += \
    quick_renderer/main.qml

RESOURCES += \
    renderer.qrc
