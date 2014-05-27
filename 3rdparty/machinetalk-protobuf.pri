# Build machinetalk protobuf files with dependencies
machinetalk-protobuf.commands = $(MAKE) all -C $$PWD/machinetalk-protobuf CXXGEN=$$OUT_PWD/generated PYGEN=$$OUT_PWD/python OBJDIR=$$OUT_PWD/objects
machinetalk-protobuf.CONFIG += no_link no_clean
!isEmpty(PROTOBUF_INCLUDE_PATH): machinetalk-protobuf.commands += GPBINCLUDE=$$PROTOBUF_INCLUDE_PATH
!isEmpty(PROTOBUF_PROTOC): machinetalk-protobuf.commands += PROTOC=$$PROTOBUF_PROTOC
ios: machinetalk-protobuf.commands += ios_replace
QMAKE_EXTRA_TARGETS += machinetalk-protobuf
PRE_TARGETDEPS += machinetalk-protobuf
QMAKE_CLEAN += -r $$OUT_PWD/generated/ $$OUT_PWD/objects/ $$OUT_PWD/python/

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
    $$OUT_PWD/generated/task.pb.cc \
    $$OUT_PWD/generated/test.pb.cc \
    $$OUT_PWD/generated/types.pb.cc \
    $$OUT_PWD/generated/value.pb.cc

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
