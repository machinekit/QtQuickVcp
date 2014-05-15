#Directories

#TOP = /home/alexander/projects/
#LINUXCNCDIR = $$TOP/linuxcnc

# eventually import directly from linuxcnc build dir
#PROTOGEN = $$LINUXCNCDIR/src/middleware/generated

include(../paths.pri)

android: {
LIBS += -L$$ZEROMQ_ANDROID_DIR/lib \
        -L$$PROTOBUF_ANDROID_DIR/src/.libs
INCLUDEPATH += $$ZEROMQ_ANDROID_DIR/include \
            $$PROTOBUF_ANDROID_DIR/src
}
mac: {
LIBS += -L$$ZEROMQ_MAC_DIR/lib
INCLUDEPATH += $$ZEROMQ_MAC_DIR/include
}

include(../3rdparty/nzmqt/nzmqt.pri)

LIBS += -lzmq
