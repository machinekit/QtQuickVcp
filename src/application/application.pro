TEMPLATE = lib
QT += qml quick network

uri = Machinekit.Application
include(../plugin.pri)

include(../zeromq.pri)
include(../../3rdparty/machinetalk-protobuf-qt/machinetalk-protobuf-lib.pri)
include(../../3rdparty/qftp/qftp.pri)

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
    qapplicationlauncher.cpp \
    qlocalsettings.cpp \
    qapplicationfilemodel.cpp \
    qapplicationfileitem.cpp \
    qapplicationplugins.cpp \
    qapplicationpluginitem.cpp \
    fileio.cpp

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
    qapplicationlauncher.h \
    qlocalsettings.h \
    qapplicationfilemodel.h \
    qapplicationfileitem.h \
    qapplicationplugins.h \
    qapplicationpluginitem.h \
    fileio.h \
    revisionsingleton.h \
    revision.h

RESOURCES += \
    application.qrc

QML_FILES = \
    ApplicationAction.qml \
    ApplicationCore.qml \
    ApplicationHelper.qml \
    ApplicationItem.qml \
    ApplicationObject.qml \
    ApplicationSettings.qml \
    MdiHistory.qml \
    HomeAllAxesHelper.qml

QML_INFRA_FILES = \
    $$QML_FILES \
    qmldir

include(../deployment.pri)
