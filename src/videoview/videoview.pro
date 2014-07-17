TEMPLATE = lib
TARGET = machinekitvideoviewplugin
TARGETPATH = Machinekit/VideoView
QT += qml quick
CONFIG += qt plugin
CONFIG -= android_install


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
    plugin.cpp \
    qmjpegstreamerclient.cpp

HEADERS += \
    plugin.h \
    qmjpegstreamerclient.h

QML_INFRA_FILES = \
    qmldir

QML_DESIGNER_FILES = \
    designer

include(../deployment.pri)
