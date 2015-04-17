TEMPLATE = lib
QT += qml quick network

uri = Machinekit.Launcher
include(../plugin.pri)

include(../zeromq.pri)
include(../../3rdparty/machinetalk-protobuf-qt/machinetalk-protobuf-lib.pri)
include(../common/common.pri)

# Input
SOURCES += \
    plugin.cpp \
    qlauncher.cpp

HEADERS += \
    plugin.h \
    qlauncher.h \
    debughelper.h

QML_INFRA_FILES = \
    qmldir

include(../deployment.pri)

RESOURCES += \
    launcher.qrc
