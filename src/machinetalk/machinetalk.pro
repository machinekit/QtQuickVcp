#-------------------------------------------------
#
# Project created by QtCreator 2016-11-01T20:31:12
#
#-------------------------------------------------

QT       += qml network
QT       -= gui
CONFIG -= android_install
CONFIG += static


TARGET = machinetalk
TEMPLATE = lib

DEFINES += MACHINETALK_LIBRARY

SOURCES += $$PWD/machinetalk/rpcclient.cpp \
           $$PWD/machinetalk/subscribe.cpp \
           $$PWD/halremote/remotecomponentbase.cpp \
           $$PWD/halremote/halrcompsubscribe.cpp

HEADERS += $$PWD/machinetalk/rpcclient.h \
           $$PWD/machinetalk/subscribe.h \
           $$PWD/halremote/remotecomponentbase.h \
           $$PWD/halremote/halrcompsubscribe.h \
        machinetalk_global.h

#macx | win32: {
#    CONFIG += static
#}

include(../../paths.pri)
include(../zeromq.pri)
include(../../3rdparty/machinetalk-protobuf-qt/machinetalk-protobuf-lib.pri)
include(../common/common.pri)

win32: target.path = $$[QT_INSTALL_BINS]
!win32: target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target
