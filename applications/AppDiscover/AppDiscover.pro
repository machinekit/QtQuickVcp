TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# deactivate installs for applications
INSTALLS =

include(../../paths.pri)
LIBS += -L$$PROTOBUF_LIB_PATH

LIBS += -lprotobuf

LIBS += -L$$ZEROMQ_LIB_PATH
LIBS += -L/Users/strahlex/libzmq-ios/libsodium-ios/libsodium_dist/lib
LIBS += -lzmq -lsodium
