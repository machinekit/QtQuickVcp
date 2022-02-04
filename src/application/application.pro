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
    plugin/plugin.cpp \
    fileio.cpp \
    applicationcommand.cpp \
    applicationconfig.cpp \
    applicationconfigfilter.cpp \
    applicationconfigitem.cpp \
    applicationdescription.cpp \
    applicationerror.cpp \
    applicationfile.cpp \
    applicationfile2.cpp \
    applicationfileitem.cpp \
    applicationfilemodel.cpp \
    applicationlauncher.cpp \
    applicationpluginitem.cpp \
    applicationplugins.cpp \
    applicationstatus.cpp \
    localsettings.cpp \
    applicationtranslator.cpp \
    filewatcher.cpp \
    applicationhelpers.cpp \
    applicationlog.cpp \
    applicationlogmessage.cpp \
    applicationfilesynchandler.cpp

HEADERS += \
    plugin/plugin.h \
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
    applicationfile2.h \
    applicationfileitem.h \
    applicationfilemodel.h \
    applicationlauncher.h \
    applicationpluginitem.h \
    applicationplugins.h \
    applicationstatus.h \
    localsettings.h \
    applicationtranslator.h \
    filewatcher.h \
    applicationhelpers.h \
    applicationlog.h \
    applicationlogmessage.h \
    applicationfilesynchandler.h

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
    plugin/qmldir

include(../deployment.pri)
