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
    plugin.cpp \
    qservice.cpp \
    qservicelist.cpp \
    qservicediscoveryfilter.cpp \
    qservicediscovery.cpp \
    qservicediscoveryitem.cpp \
    qnameserver.cpp \
    qservicediscoveryquery.cpp

HEADERS += \
    plugin.h \
    qservice.h \
    qservicelist.h \
    qservicediscoveryfilter.h \
    qservicediscovery.h \
    qservicediscoveryitem.h \
    qnameserver.h \
    debughelper.h \
    qservicediscoveryquery.h

QML_INFRA_FILES = \
    qmldir

include(../deployment.pri)

JAVAFILES = android-sources/src

DISTFILES += \
    android-sources/src/io/machinekit/service/MulticastActivator.java
