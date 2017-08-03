include(../../paths.pri)

MACHINETALK_PATH = $$OUT_PWD/../../3rdparty/machinetalk-protobuf-qt
INCLUDEPATH += $$MACHINETALK_PATH/build/cpp
!isEmpty(PROTOBUF_INCLUDE_PATH): INCLUDEPATH += $$PROTOBUF_INCLUDE_PATH

!win32 {
    LIBS += -L$$MACHINETALK_PATH
} else {
    CONFIG(debug, release): LIBS += -L$$MACHINETALK_PATH/release
    CONFIG(debug, debug): LIBS += -L$$MACHINETALK_PATH/debug
    !isEmpty(PROTOBUF_LIB_PATH): LIBS += -L$$PROTOBUF_LIB_PATH
    !isEmpty(PROTOBUF_LIB_FLAGS): LIBS += $$PROTOBUF_LIB_FLAGS
    LIBS += -llibprotobuf
}
macx: {
    !isEmpty(PROTOBUF_LIB_PATH): LIBS += -L$$PROTOBUF_LIB_PATH
    !isEmpty(PROTOBUF_LIB_FLAGS): LIBS += $$PROTOBUF_LIB_FLAGS
    LIBS += -lprotobuf
    LIBS += -Bstatic
}
LIBS += -lmachinetalk-pb2
