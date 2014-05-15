machinetalk-protobuf.commands = make -C $$PWD/machinetalk-protobuf
QMAKE_EXTRA_TARGETS += machinetalk-protobuf
PRE_TARGETDEPS += machinetalk-protobuf

INCLUDEPATH += $$PWD/machinetalk-protobuf/generated

android: {
    LIBS += -L$$PROTOBUF_ANDROID_DIR/lib
    INCLUDEPATH += $$PROTOBUF_ANDROID_DIR/include
}

mac: {
    LIBS += -L$$PROTOBUF_MAC_DIR/lib
    INCLUDEPATH += $$PROTOBUF_MAC_DIR/include
}

LIBS += -lprotobuf

HEADERS += \
    $$PWD/machinetalk-protobuf/generated/canon.pb.h \
    $$PWD/machinetalk-protobuf/generated/config.pb.h \
    $$PWD/machinetalk-protobuf/generated/emcclass.pb.h \
    $$PWD/machinetalk-protobuf/generated/log.pb.h \
    $$PWD/machinetalk-protobuf/generated/message.pb.h \
    $$PWD/machinetalk-protobuf/generated/motcmds.pb.h \
    $$PWD/machinetalk-protobuf/generated/nanopb.pb.h \
    $$PWD/machinetalk-protobuf/generated/object.pb.h \
    $$PWD/machinetalk-protobuf/generated/preview.pb.h \
    $$PWD/machinetalk-protobuf/generated/rtapi_message.pb.h \
    $$PWD/machinetalk-protobuf/generated/rtapicommand.pb.h \
    $$PWD/machinetalk-protobuf/generated/task.pb.h \
    $$PWD/machinetalk-protobuf/generated/test.pb.h \
    $$PWD/machinetalk-protobuf/generated/types.pb.h \
    $$PWD/machinetalk-protobuf/generated/value.pb.h

SOURCES += \
    $$PWD/machinetalk-protobuf/generated/canon.pb.cc \
    $$PWD/machinetalk-protobuf/generated/config.pb.cc \
    $$PWD/machinetalk-protobuf/generated/emcclass.pb.cc \
    $$PWD/machinetalk-protobuf/generated/log.pb.cc \
    $$PWD/machinetalk-protobuf/generated/message.pb.cc \
    $$PWD/machinetalk-protobuf/generated/motcmds.pb.cc \
    $$PWD/machinetalk-protobuf/generated/nanopb.pb.cc \
    $$PWD/machinetalk-protobuf/generated/object.pb.cc \
    $$PWD/machinetalk-protobuf/generated/preview.pb.cc \
    $$PWD/machinetalk-protobuf/generated/rtapi_message.pb.cc \
    $$PWD/machinetalk-protobuf/generated/rtapicommand.pb.cc \
    $$PWD/machinetalk-protobuf/generated/task.pb.cc \
    $$PWD/machinetalk-protobuf/generated/test.pb.cc \
    $$PWD/machinetalk-protobuf/generated/types.pb.cc \
    $$PWD/machinetalk-protobuf/generated/value.pb.cc

OTHER_FILES += \
    $$PWD/machinetalk-protobuf/proto/canon.proto \
    $$PWD/machinetalk-protobuf/proto/config.proto \
    $$PWD/machinetalk-protobuf/proto/emcclass.proto \
    $$PWD/machinetalk-protobuf/proto/log.proto \
    $$PWD/machinetalk-protobuf/proto/message.proto \
    $$PWD/machinetalk-protobuf/proto/motcmds.proto \
    $$PWD/machinetalk-protobuf/proto/nanopb.proto \
    $$PWD/machinetalk-protobuf/proto/object.proto \
    $$PWD/machinetalk-protobuf/proto/preview.proto \
    $$PWD/machinetalk-protobuf/proto/rtapi_message.proto \
    $$PWD/machinetalk-protobuf/proto/rtapicommand.proto \
    $$PWD/machinetalk-protobuf/proto/task.proto \
    $$PWD/machinetalk-protobuf/proto/test.proto \
    $$PWD/machinetalk-protobuf/proto/types.proto \
    $$PWD/machinetalk-protobuf/proto/value.proto
