#Directories

#TOP = /home/alexander/projects/
#LINUXCNCDIR = $$TOP/linuxcnc

# eventually import directly from linuxcnc build dir
#PROTOGEN = $$LINUXCNCDIR/src/middleware/generated

NZMQTDIR = ../../3rdparty/nzmqt
include(../paths.pri)

# This define will "move" nzmqt class method
# implementations to nzmqt.cpp file.
DEFINES += NZMQT_LIB

SOURCES += \
    $$NZMQTDIR/src/nzmqt/nzmqt.cpp

HEADERS += \
    $$NZMQTDIR/include/nzmqt/global.hpp \
    $$NZMQTDIR/include/nzmqt/nzmqt.hpp \
    $$NZMQTDIR/include/nzmqt/impl.hpp

INCLUDEPATH += \
    $$NZMQTDIR/include \
    $$NZMQTDIR/externals/include

android: {
LIBS += -L$$ZEROMQ_ANDROID_DIR/lib/ \
        -L$$PROTOBUF_ANDROID_DIR/src/.libs/
INCLUDEPATH += $$ZEROMQ_ANDROID_DIR/include/ \
            $$PROTOBUF_ANDROID_DIR/src/
}

LIBS += -lzmq -lprotobuf 
