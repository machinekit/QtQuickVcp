TEMPLATE = lib
QT += qml quick

uri = Machinekit.HalRemote
include(../plugin.pri)

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
