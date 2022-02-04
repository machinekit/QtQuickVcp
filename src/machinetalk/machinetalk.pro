#-------------------------------------------------
#
# Project created by QtCreator 2016-11-01T20:31:12
#
#-------------------------------------------------

QT       += qml network
QT       -= gui
CONFIG += static
CONFIG += c++14

TARGET = machinetalk
TEMPLATE = lib

DEFINES += MACHINETALK_LIBRARY

SOURCES += $$PWD/common/rpcclient.cpp \
           $$PWD/common/subscribe.cpp \
           $$PWD/common/publish.cpp \
           $$PWD/common/simplesubscribe.cpp \
           $$PWD/halremote/remotecomponentbase.cpp \
           $$PWD/halremote/halrcompsubscribe.cpp \
           $$PWD/application/launchersubscribe.cpp \
           $$PWD/application/launcherbase.cpp \
           $$PWD/application/configbase.cpp \
           $$PWD/application/errorsubscribe.cpp \
           $$PWD/application/errorbase.cpp \
           $$PWD/application/statussubscribe.cpp \
           $$PWD/application/statusbase.cpp \
           $$PWD/application/commandbase.cpp \
           $$PWD/application/logbase.cpp \
           $$PWD/application/logservicebase.cpp \
           $$PWD/pathview/previewclientbase.cpp \
           $$PWD/pathview/previewsubscribe.cpp \
           $$PWD/remotefile/filebase.cpp \
           $$PWD/machinetalkservice.cpp \

HEADERS += $$PWD/common/rpcclient.h \
           $$PWD/common/subscribe.h \
           $$PWD/common/publish.h \
           $$PWD/common/simplesubscribe.h \
           $$PWD/halremote/remotecomponentbase.h \
           $$PWD/halremote/halrcompsubscribe.h \
           $$PWD/application/launchersubscribe.h \
           $$PWD/application/launcherbase.h \
           $$PWD/application/configbase.h \
           $$PWD/application/errorsubscribe.h \
           $$PWD/application/errorbase.h \
           $$PWD/application/statussubscribe.h \
           $$PWD/application/statusbase.h \
           $$PWD/application/commandbase.h \
           $$PWD/application/logbase.h \
           $$PWD/application/logservicebase.h \
           $$PWD/pathview/previewclientbase.h \
           $$PWD/pathview/previewsubscribe.h \
           $$PWD/remotefile/filebase.h \
           $$PWD/machinetalkservice.h \
           $$PWD/machinetalk_global.h

include(../../paths.pri)
include(../zeromq.pri)
include(../../3rdparty/machinetalk-protobuf-qt/machinetalk-protobuf-lib.pri)
include(../common/common.pri)

win32: target.path = $$[QT_INSTALL_BINS]
!win32: target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target
