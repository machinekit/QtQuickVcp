QT     -= core gui
CONFIG -= android_install

macx | win32: {
    CONFIG += static
}

TARGET = machinetalk-pb2
TEMPLATE = lib

include(../../paths.pri)

PROTOGEN = build/cpp

# Protobuf
MACHINETALK_DIR = $$PWD/../machinetalk-protobuf/src/
NAMESPACE_DIR = machinetalk/protobuf
PROTO_DIR = $$MACHINETALK_DIR/$$NAMESPACE_DIR
PROTOS += \
    $$PROTO_DIR/nanopb.proto \
    $$PROTO_DIR/types.proto \
    $$PROTO_DIR/emcclass.proto \
    $$PROTO_DIR/motcmds.proto \
    $$PROTO_DIR/object.proto \
    $$PROTO_DIR/task.proto \
    $$PROTO_DIR/value.proto \
    $$PROTO_DIR/rtapicommand.proto \
    $$PROTO_DIR/rtapi_message.proto \
    $$PROTO_DIR/canon.proto \
    $$PROTO_DIR/config.proto \
    $$PROTO_DIR/log.proto \
    $$PROTO_DIR/preview.proto \
    $$PROTO_DIR/status.proto \
    $$PROTO_DIR/message.proto \
    $$PROTO_DIR/test.proto

PROTOPATH += $$MACHINETALK_DIR
include(../../src/protobuf.pri)

win32: target.path = $$[QT_INSTALL_BINS]
!win32: target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target
