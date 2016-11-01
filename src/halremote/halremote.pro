TEMPLATE = lib
QT += qml quick network

uri = Machinekit.HalRemote
include(../plugin.pri)

include(../zeromq.pri)
include(../../3rdparty/machinetalk-protobuf-qt/machinetalk-protobuf-lib.pri)
include(../common/common.pri)

# Input
SOURCES += \
    plugin.cpp \
    halgroup.cpp \
    halpin.cpp \
    halremotecomponent.cpp \
    halsignal.cpp

HEADERS += \
    plugin.h \
    debughelper.h \
    halgroup.h \
    halpin.h \
    halremotecomponent.h \
    halsignal.h

QML_INFRA_FILES = \
    qmldir

include(../deployment.pri)
