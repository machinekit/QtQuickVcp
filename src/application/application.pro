TEMPLATE = lib
QT += qml quick network

uri = Machinekit.Application
include(../plugin.pri)

include(../zeromq.pri)
include(../../3rdparty/machinetalk-protobuf-qt/machinetalk-protobuf-lib.pri)
include(../../3rdparty/qftp/qftp.pri)

include(../common/common.pri)
include(../machinetalk/machinetalk.pri)

# Input
SOURCES += \
    plugin.cpp \
    fileio.cpp \
    applicationcommand.cpp \
    applicationconfig.cpp \
    applicationconfigfilter.cpp \
    applicationconfigitem.cpp \
    applicationdescription.cpp \
    applicationerror.cpp \
    applicationfile.cpp \
    applicationfileitem.cpp \
    applicationfilemodel.cpp \
    applicationlauncher.cpp \
    applicationpluginitem.cpp \
    applicationplugins.cpp \
    applicationstatus.cpp \
    localsettings.cpp \
    applicationtranslator.cpp

HEADERS += \
    plugin.h \
    fileio.h \
    revisionsingleton.h \
    revision.h \
    applicationcommand.h \
    applicationconfig.h \
    applicationconfigfilter.h \
    applicationconfigitem.h \
    applicationdescription.h \
    applicationerror.h \
    applicationfile.h \
    applicationfileitem.h \
    applicationfilemodel.h \
    applicationlauncher.h \
    applicationpluginitem.h \
    applicationplugins.h \
    applicationstatus.h \
    localsettings.h \
    applicationtranslator.h

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
    HomeAllAxesHelper.qml \
    QueuedConnection.qml

QML_INFRA_FILES = \
    $$QML_FILES \
    qmldir

include(../deployment.pri)
