TEMPLATE = lib
TARGET = machinekitvideoviewplugin
TARGETPATH = Machinekit/VideoView
QT += qml quick
CONFIG += qt plugin


TARGET = $$qtLibraryTarget($$TARGET)
uri = Machinekit.VideoView
QMAKE_MOC_OPTIONS += -Muri=$$uri

include(../zeromq.pri)

# Protobuf
PROTOPATH += $$PWD/proto
PROTOS_IN += proto/package.proto
include(../protobuf.pri)

# Input
SOURCES += \
    mjpegstreamerclient.cpp \
    plugin.cpp

HEADERS += \
    mjpegstreamerclient.h \
    plugin.h

QML_INFRA_FILES = \
    qmldir

QML_DESIGNER_FILES = \
    designer

QML_PLUGINDUMP_DUMMY = \
    qmldir

include(../deployment.pri)
