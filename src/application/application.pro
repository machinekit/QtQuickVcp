TEMPLATE = lib
QT += qml quick

uri = Machinekit.Application
include(../plugin.pri)

include(../zeromq.pri)
include(../../3rdparty/machinetalk-protobuf-qt/machinetalk-protobuf-lib.pri)

include(../common/common.pri)

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
    qapplicationfile.cpp \
    qlocalsettings.cpp

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
    qapplicationfile.h \
    qlocalsettings.h

RESOURCES += \
    application.qrc

QML_FILES = \
    ApplicationAction.qml \
    ApplicationCore.qml \
    ApplicationItem.qml \
    ApplicationObject.qml \
    ApplicationSettings.qml \
    MdiHistory.qml

QML_INFRA_FILES = \
    $$QML_FILES \
    qmldir

include(../deployment.pri)
