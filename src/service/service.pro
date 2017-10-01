TEMPLATE = lib
QT += qml quick network

android: {
    QT += androidextras
}

uri = Machinekit.Service
include(../plugin.pri)

include(../../3rdparty/jdns/jdns.pri)

# Input
SOURCES += \
    plugin/plugin.cpp \
    nameserver.cpp \
    service.cpp \
    servicediscovery.cpp \
    servicediscoveryfilter.cpp \
    servicediscoveryitem.cpp \
    servicediscoveryquery.cpp \
    servicelist.cpp

HEADERS += \
    plugin/plugin.h \
    nameserver.h \
    service.h \
    servicediscovery.h \
    servicediscoveryfilter.h \
    servicediscoveryitem.h \
    servicediscoveryquery.h \
    servicelist.h

QML_INFRA_FILES = \
    plugin/qmldir

include(../deployment.pri)

JAVAFILES = android-sources/src

DISTFILES += \
    android-sources/src/io/machinekit/service/MulticastActivator.java
