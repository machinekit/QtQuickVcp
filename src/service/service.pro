TEMPLATE = lib
TARGET = machinekitserviceplugin
TARGETPATH = Machinekit/Service
QT += qml quick
CONFIG += qt plugin
CONFIG -= android_install

TARGET = $$qtLibraryTarget($$TARGET)
uri = Machinekit.Service
QMAKE_MOC_OPTIONS += -Muri=$$uri

include(../../3rdparty/jdns/jdns.pri)

# Input
SOURCES += \
    plugin.cpp \
    qservice.cpp \
    qservicelist.cpp \
    qservicediscoveryfilter.cpp \
    qservicediscovery.cpp \
    qservicediscoveryitem.cpp \
    qnameserver.cpp

HEADERS += \
    plugin.h \
    qservice.h \
    qservicelist.h \
    qservicediscoveryfilter.h \
    qservicediscovery.h \
    qservicediscoveryitem.h \
    qnameserver.h \
    debughelper.h

QML_INFRA_FILES = \
    qmldir

include(../deployment.pri)
