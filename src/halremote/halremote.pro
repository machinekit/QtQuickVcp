TEMPLATE = lib
TARGET = machinekithalremoteplugin
TARGETPATH = Machinekit/HalRemote
QT += qml quick
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = Machinekit.HalRemote
QMAKE_MOC_OPTIONS += -Muri=$$uri

include(../zeromq.pri)
include(../../3rdparty/jdns/jdns.pri)
include(../../3rdparty/machinetalk-protobuf.pri)

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
    qappdiscoveryitem.cpp

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
    qappdiscoveryitem.h

QML_INFRA_FILES = \
    qmldir

include(../deployment.pri)
