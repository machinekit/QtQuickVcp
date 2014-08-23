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
include(../../3rdparty/jdns/jdns.pri)
include(../../3rdparty/machinetalk-protobuf.pri)

# Input
SOURCES += \
    plugin.cpp \
    qservice.cpp \
    qhalpin.cpp \
    qhalremotecomponent.cpp \
    qservicelist.cpp \
    qservicediscoveryfilter.cpp \
    qservicediscovery.cpp \
    qservicediscoveryitem.cpp \
    qapplicationconfig.cpp \
    qapplicationconfigitem.cpp \
    qapplicationconfigfilter.cpp \
    qapplicationdescription.cpp \
    qnameserver.cpp \
    qhalgroup.cpp \
    qhalsignal.cpp \
    qemcstatus.cpp \
    qemccommand.cpp \
    qemcerror.cpp

HEADERS += \
    plugin.h \
    qservice.h \
    qhalpin.h \
    qhalremotecomponent.h \
    qservicelist.h \
    qservicediscoveryfilter.h \
    qservicediscovery.h \
    qservicediscoveryitem.h \
    qapplicationconfig.h \
    qapplicationconfigitem.h \
    qapplicationconfigfilter.h \
    debughelper.h \
    qapplicationdescription.h \
    qnameserver.h \
    qhalgroup.h \
    qhalsignal.h \
    qemcstatus.h \
    qemccommand.h \
    qemcerror.h

QML_INFRA_FILES = \
    qmldir

include(../deployment.pri)
