TEMPLATE = lib
QT += qml quick

uri = Machinekit.Application
include(../plugin.pri)

include(../zeromq.pri)
include(../../3rdparty/machinetalk-protobuf-qt/machinetalk-protobuf-lib.pri)

# Input
SOURCES += \
    plugin.cpp \
    qapplicationconfig.cpp \
    qapplicationconfigitem.cpp \
    qapplicationconfigfilter.cpp \
    qapplicationdescription.cpp \
    qapplicationcommand.cpp \
    qapplicationerror.cpp \
    qapplicationstatus.cpp \
    qapplicationfile.cpp

HEADERS += \
    plugin.h \
    qapplicationconfig.h \
    qapplicationconfigitem.h \
    qapplicationconfigfilter.h \
    debughelper.h \
    qapplicationdescription.h \
    qapplicationcommand.h \
    qapplicationerror.h \
    qapplicationstatus.h \
    qapplicationfile.h

QML_INFRA_FILES = \
    qmldir

include(../deployment.pri)
