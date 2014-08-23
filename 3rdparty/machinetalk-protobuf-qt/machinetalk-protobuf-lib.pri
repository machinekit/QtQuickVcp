MACHINETALK_PATH = $$OUT_PWD/../../3rdparty/machinetalk-protobuf-qt
INCLUDEPATH += $$MACHINETALK_PATH/generated
!isEmpty(PROTOBUF_INCLUDE_PATH): INCLUDEPATH += $$PROTOBUF_INCLUDE_PATH

LIBS += -L$$MACHINETALK_PATH
LIBS += -lmachinetalk-protobuf
