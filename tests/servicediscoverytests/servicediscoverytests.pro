#-------------------------------------------------
#
# Project created by QtCreator 2017-08-04T13:53:49
#
#-------------------------------------------------

QT       += network testlib qml

QT       -= gui

TARGET = tst_servicediscoverytest
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SERVICE_PATH = $$PWD/../../src/service/
INCLUDEPATH += $$SERVICE_PATH
HEADERS += \
    $$SERVICE_PATH/nameserver.h \
    $$SERVICE_PATH/service.h \
    $$SERVICE_PATH/servicediscovery.h \
    $$SERVICE_PATH/servicediscoveryfilter.h \
    $$SERVICE_PATH/servicediscoveryitem.h \
    $$SERVICE_PATH/servicediscoveryquery.h \
    $$SERVICE_PATH/servicelist.h
SOURCES += \
    $$SERVICE_PATH/nameserver.cpp \
    $$SERVICE_PATH/service.cpp \
    $$SERVICE_PATH/servicediscovery.cpp \
    $$SERVICE_PATH/servicediscoveryfilter.cpp \
    $$SERVICE_PATH/servicediscoveryitem.cpp \
    $$SERVICE_PATH/servicediscoveryquery.cpp \
    $$SERVICE_PATH/servicelist.cpp

SOURCES += \
        tst_servicediscoverytest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
