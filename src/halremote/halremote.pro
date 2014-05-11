TEMPLATE = lib
TARGET = machinekithalremoteplugin
TARGETPATH = Machinekit/HalRemote
QT += qml quick
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = Machinekit.HalRemote

include(../zeromq.pri)
include(../../3rdparty/jdns/jdns.pri)
include(../../3rdparty/Machinekit/proto/machinekitproto.pri)

# Input
SOURCES += \
    plugin.cpp \
    qappconfig.cpp \
    qappconfigfilter.cpp \
    qappconfigitem.cpp \
    qservice.cpp \
    qservicediscovery.cpp \
    qhalpin.cpp \
    qhalremotecomponent.cpp \
    qappdiscovery.cpp \
    qappdiscoveryitem.cpp \
    qhalapplicationbase.cpp

HEADERS += \
    plugin.h \
    qappconfig.h \
    qappconfigfilter.h \
    qappconfigitem.h \
    qservice.h \
    qservicediscovery.h \
    qhalpin.h \
    qhalremotecomponent.h \
    qappdiscovery.h \
    qappdiscoveryitem.h \
    qhalapplicationbase.h

QML_INFRA_FILES = \
    qmldir

include(../deployment.pri)
