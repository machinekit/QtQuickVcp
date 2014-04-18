TEMPLATE = lib
TARGET = machinekitvideoviewplugin
TARGETPATH = Machinekit/VideoView
QT += qml quick
CONFIG += qt plugin


TARGET = $$qtLibraryTarget($$TARGET)
uri = Machinekit.VideoView


include(../zmqproto.pri)

#Directories

# for now, use included pre-protoc'ed includes and C++ files:
PROTOGEN = generated

# Input
SOURCES += \
    mjpegstreamerclient.cpp \
    $$PROTOGEN/package.pb.cc \
    plugin.cpp

HEADERS += \
    mjpegstreamerclient.h \
    $$PROTOGEN/package.pb.h \
    plugin.h

INCLUDEPATH += \
    $$PROTOGEN

QML_INFRA_FILES += \
    qmldir
#    src/plugins.qmltypes

include(../deployment.pri)
