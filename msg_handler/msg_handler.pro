
QT_VERSION = 5

QT += core gui websockets

ROOT_DIRECTORY = ../../al_build/msg_handler
LIB_DIRECTORY = $${ROOT_DIRECTORY}/lib
BUILD_DIRECTORY = $${ROOT_DIRECTORY}/build
TESTS_DIRECTORY = $${BUILD_DIRECTORY}/tests
EXAMPLES_DIRECTORY = $${BUILD_DIRECTORY}/examples
EXEC_DIRECTORY = $${BUILD_DIRECTORY}

DESTDIR = $${EXEC_DIRECTORY}
OBJECTS_DIR = $${BUILD_DIRECTORY}
MOC_DIR = $${BUILD_DIRECTORY}
RCC_DIR = $${BUILD_DIRECTORY}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = msg-handler
TEMPLATE = lib
CONFIG += plugin

QMAKE_CXXFLAGS += -fPIC

INCLUDEPATH+=../AlteXo/interfaces/

SOURCES += \
    alwsplugin.cpp \
    alwsclient.cpp

HEADERS += \
    alwsplugin.h \
    alwsclient.h
