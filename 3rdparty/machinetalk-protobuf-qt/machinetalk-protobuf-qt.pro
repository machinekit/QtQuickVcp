QT     -= core gui
CONFIG -= android_install
CONFIG += release

win32: {
    CONFIG += static
}

TARGET = machinetalk-protobuf
TEMPLATE = lib

include(../../paths.pri)

# Protobuf
MACHINETALK_DIR = $$PWD/../machinetalk-protobuf
PROTOPATH = $$PWD/proto
PROTOS += \
    $$MACHINETALK_DIR/proto/nanopb.proto \
    $$MACHINETALK_DIR/proto/types.proto \
    $$MACHINETALK_DIR/proto/emcclass.proto \
    $$MACHINETALK_DIR/proto/motcmds.proto \
    $$MACHINETALK_DIR/proto/object.proto \
    $$MACHINETALK_DIR/proto/task.proto \
    $$MACHINETALK_DIR/proto/value.proto \
    $$MACHINETALK_DIR/proto/rtapicommand.proto \
    $$MACHINETALK_DIR/proto/rtapi_message.proto \
    $$MACHINETALK_DIR/proto/canon.proto \
    $$MACHINETALK_DIR/proto/config.proto \
    $$MACHINETALK_DIR/proto/log.proto \
    $$MACHINETALK_DIR/proto/preview.proto \
    $$MACHINETALK_DIR/proto/status.proto \
    $$MACHINETALK_DIR/proto/message.proto \
    $$MACHINETALK_DIR/proto/test.proto

include(../../src/protobuf.pri)

win32: target.path = $$[QT_INSTALL_BINS]
!win32: target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target
