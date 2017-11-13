#-------------------------------------------------
#
# Project created by QtCreator 2017-01-23T11:15:23
#
#-------------------------------------------------

QT       += network testlib qml
QT       -= gui

TARGET = applicationtests
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++14

TEMPLATE = app

include(../../3rdparty/catch/catch.pri)
include(../../3rdparty/trompeloeil/trompeloeil.pri)

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../../src/zeromq.pri)
include(../../3rdparty/machinetalk-protobuf-qt/machinetalk-protobuf-lib.pri)
include(../../src/common/common.pri)
include(../../src/machinetalk/machinetalk.pri)
include(../../src/protobuf.pri)
include(../../3rdparty/qftp/qftp.pri)

APPLICATION_PATH = $$PWD/../../src/application/
INCLUDEPATH += $$APPLICATION_PATH
HEADERS += $$APPLICATION_PATH/filewatcher.h \
    $$APPLICATION_PATH/applicationlog.h \
    $$APPLICATION_PATH/applicationlogmessage.h \
    $$APPLICATION_PATH/applicationfile.h \
    $$APPLICATION_PATH/applicationfileitem.h \
    $$APPLICATION_PATH/applicationfilemodel.h \
    logservice.h

SOURCES += $$APPLICATION_PATH/filewatcher.cpp \
    $$APPLICATION_PATH/applicationlog.cpp \
    $$APPLICATION_PATH/applicationlogmessage.cpp \
    $$APPLICATION_PATH/applicationfile.cpp \
    $$APPLICATION_PATH/applicationfileitem.cpp \
    $$APPLICATION_PATH/applicationfilemodel.cpp \
    logservice.cpp

SOURCES += \
    test_filewatcher.cpp \
    test_applicationlog.cpp \
    test_applicationfile.cpp \
    main.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
