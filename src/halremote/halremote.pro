TEMPLATE = lib
TARGET = machinekithalremoteplugin
TARGETPATH = Machinekit/HalRemote
QT += qml quick
CONFIG += qt plugin
CONFIG -= android_install

TARGET = $$qtLibraryTarget($$TARGET)
uri = Machinekit.HalRemote
QMAKE_MOC_OPTIONS += -Muri=$$uri

include(../zeromq.pri)
include(../../3rdparty/machinetalk-protobuf-qt/machinetalk-protobuf-lib.pri)

# Input
SOURCES += \
    plugin.cpp \
    qhalpin.cpp \
    qhalremotecomponent.cpp \
    qhalgroup.cpp \
    qhalsignal.cpp \

HEADERS += \
    plugin.h \
    qhalpin.h \
    qhalremotecomponent.h \
    qhalgroup.h \
    qhalsignal.h \
    debughelper.h

QML_INFRA_FILES = \
    qmldir

include(../deployment.pri)
