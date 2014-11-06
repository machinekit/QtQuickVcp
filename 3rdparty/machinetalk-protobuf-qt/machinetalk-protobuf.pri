# Build machinetalk protobuf files with dependencies
MACHINETALK_DIR = $$PWD/../machinetalk-protobuf
!win32: {
    machinetalk-protobuf.commands = $(MAKE) all -C $$MACHINETALK_DIR \
        -e CXXGEN=$$OUT_PWD/generated \
        -e PYGEN=$$OUT_PWD/python \
        -e OBJDIR=$$OUT_PWD/objects
    machinetalk-protobuf.CONFIG += no_link no_clean
    !isEmpty(PROTOBUF_INCLUDE_PATH): machinetalk-protobuf.commands += -e GPBINCLUDE=$$PROTOBUF_INCLUDE_PATH
    !isEmpty(PROTOBUF_PROTOC): machinetalk-protobuf.commands += -e PROTOC=$$PROTOBUF_PROTOC
    ios: machinetalk-protobuf.commands += ios_replace
} else {
    machinetalk-protobuf.commands = $(COPY_DIR) $$shell_path($$MACHINETALK_DIR/generated) $$shell_path($$OUT_PWD/generated/)
    machinetalk-protobuf.CONFIG += no_link no_clean
}
QMAKE_EXTRA_TARGETS += machinetalk-protobuf
PRE_TARGETDEPS += machinetalk-protobuf

win32:QMAKE_DEL_FILE = del /q /f
!win32:QMAKE_DEL_FILE = rm -r -f
QMAKE_CLEAN += $$OUT_PWD/generated $$OUT_PWD/object $$OUT_PWD/python

INCLUDEPATH += $$OUT_PWD/generated

!isEmpty(PROTOBUF_INCLUDE_PATH): INCLUDEPATH += $$PROTOBUF_INCLUDE_PATH
!isEmpty(PROTOBUF_LIB_PATH): LIBS += -L$$PROTOBUF_LIB_PATH
!isEmpty(PROTOBUF_LIB_FLAGS): LIBS += $$PROTOBUF_LIB_FLAGS
LIBS += -lprotobuf

HEADERS += \
    $$OUT_PWD/generated/canon.pb.h \
    $$OUT_PWD/generated/config.pb.h \
    $$OUT_PWD/generated/emcclass.pb.h \
    $$OUT_PWD/generated/log.pb.h \
    $$OUT_PWD/generated/message.pb.h \
    $$OUT_PWD/generated/motcmds.pb.h \
    $$OUT_PWD/generated/nanopb.pb.h \
    $$OUT_PWD/generated/object.pb.h \
    $$OUT_PWD/generated/preview.pb.h \
    $$OUT_PWD/generated/rtapi_message.pb.h \
    $$OUT_PWD/generated/rtapicommand.pb.h \
    $$OUT_PWD/generated/status.pb.h \
    $$OUT_PWD/generated/task.pb.h \
    $$OUT_PWD/generated/test.pb.h \
    $$OUT_PWD/generated/types.pb.h \
    $$OUT_PWD/generated/value.pb.h

SOURCES += \
    $$OUT_PWD/generated/canon.pb.cc \
    $$OUT_PWD/generated/config.pb.cc \
    $$OUT_PWD/generated/emcclass.pb.cc \
    $$OUT_PWD/generated/log.pb.cc \
    $$OUT_PWD/generated/message.pb.cc \
    $$OUT_PWD/generated/motcmds.pb.cc \
    $$OUT_PWD/generated/nanopb.pb.cc \
    $$OUT_PWD/generated/object.pb.cc \
    $$OUT_PWD/generated/preview.pb.cc \
    $$OUT_PWD/generated/rtapi_message.pb.cc \
    $$OUT_PWD/generated/rtapicommand.pb.cc \
    $$OUT_PWD/generated/status.pb.cc \
    $$OUT_PWD/generated/task.pb.cc \
    $$OUT_PWD/generated/test.pb.cc \
    $$OUT_PWD/generated/types.pb.cc \
    $$OUT_PWD/generated/value.pb.cc

OTHER_FILES += \
    $$MACHINETALK_DIR/proto/canon.proto \
    $$MACHINETALK_DIR/proto/config.proto \
    $$MACHINETALK_DIR/proto/emcclass.proto \
    $$MACHINETALK_DIR/proto/log.proto \
    $$MACHINETALK_DIR/proto/message.proto \
    $$MACHINETALK_DIR/proto/motcmds.proto \
    $$MACHINETALK_DIR/proto/nanopb.proto \
    $$MACHINETALK_DIR/proto/object.proto \
    $$MACHINETALK_DIR/proto/preview.proto \
    $$MACHINETALK_DIR/proto/rtapi_message.proto \
    $$MACHINETALK_DIR/proto/rtapicommand.proto \
    $$MACHINETALK_DIR/proto/status.proto \
    $$MACHINETALK_DIR/proto/task.proto \
    $$MACHINETALK_DIR/proto/test.proto \
    $$MACHINETALK_DIR/proto/types.proto \
    $$MACHINETALK_DIR/proto/value.proto \
    $$MACHINETALK_DIR/Makefile
