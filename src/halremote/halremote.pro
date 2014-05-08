TEMPLATE = lib
TARGET = machinekithalremoteplugin
TARGETPATH = Machinekit/HalRemote
QT += qml quick
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = Machinekit.HalRemote

# for now, use included pre-protoc'ed includes and C++ files:
PROTOGEN = generated

include(../zmqproto.pri)
include(../../3rdparty/jdns/jdns.pri)

# Input
SOURCES += \
    plugin.cpp \
    qappconfig.cpp \
    qappconfigfilter.cpp \
    qappconfigitem.cpp \
    qservice.cpp \
    qservicediscovery.cpp \
    $$PROTOGEN/canon.pb.cc \
    $$PROTOGEN/config.pb.cc \
    $$PROTOGEN/emcclass.pb.cc \
    $$PROTOGEN/log.pb.cc \
    $$PROTOGEN/message.pb.cc \
    $$PROTOGEN/motcmds.pb.cc \
    $$PROTOGEN/nanopb.pb.cc \
    $$PROTOGEN/object.pb.cc \
    $$PROTOGEN/rtapicommand.pb.cc \
    $$PROTOGEN/rtapi_message.pb.cc \
    $$PROTOGEN/rtmessage.pb.cc \
    $$PROTOGEN/task.pb.cc \
    $$PROTOGEN/test.pb.cc \
    $$PROTOGEN/types.pb.cc \
    $$PROTOGEN/value.pb.cc \
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
    $$PROTOGEN/canon.pb.h \
    $$PROTOGEN/emcclass.pb.h \
    $$PROTOGEN/log.pb.h \
    $$PROTOGEN/message.pb.h \
    $$PROTOGEN/motcmds.pb.h \
    $$PROTOGEN/nanopb.pb.h \
    $$PROTOGEN/object.pb.h \
    $$PROTOGEN/rtapicommand.pb.h \
    $$PROTOGEN/rtapi_message.pb.h \
    $$PROTOGEN/rtmessage.pb.h \
    $$PROTOGEN/task.pb.h \
    $$PROTOGEN/test.pb.h \
    $$PROTOGEN/types.pb.h \
    $$PROTOGEN/value.pb.h \
    qhalpin.h \
    qhalremotecomponent.h \
    qappdiscovery.h \
    qappdiscoveryitem.h

INCLUDEPATH += \
    $$PROTOGEN

QML_INFRA_FILES = \
    qmldir

include(../deployment.pri)
