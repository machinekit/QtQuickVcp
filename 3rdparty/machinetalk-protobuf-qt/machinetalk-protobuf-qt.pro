QT     -= core gui
CONFIG -= android_install
CONFIG += release

macx | win32: {
    CONFIG += static
}

TARGET = machinetalk-protobuf
TEMPLATE = lib

include(../../paths.pri)

PROTOGEN = build/cpp/machinetalk/protobuf

# Protobuf
MACHINETALK_DIR = $$PWD/../machinetalk-protobuf/proto
PROTOS += \
    $$MACHINETALK_DIR/nanopb.proto \
    $$MACHINETALK_DIR/types.proto \
    $$MACHINETALK_DIR/emcclass.proto \
    $$MACHINETALK_DIR/motcmds.proto \
    $$MACHINETALK_DIR/object.proto \
    $$MACHINETALK_DIR/task.proto \
    $$MACHINETALK_DIR/value.proto \
    $$MACHINETALK_DIR/rtapicommand.proto \
    $$MACHINETALK_DIR/rtapi_message.proto \
    $$MACHINETALK_DIR/canon.proto \
    $$MACHINETALK_DIR/config.proto \
    $$MACHINETALK_DIR/log.proto \
    $$MACHINETALK_DIR/preview.proto \
    $$MACHINETALK_DIR/status.proto \
    $$MACHINETALK_DIR/message.proto \
    $$MACHINETALK_DIR/test.proto

include(../../src/protobuf.pri)

win32: target.path = $$[QT_INSTALL_BINS]
!win32: target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target
