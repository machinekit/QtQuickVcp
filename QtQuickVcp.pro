#Directories
LINUXCNCDIR = /home/alexander/projects/linuxcnc
NZMQTDIR = /home/alexander/projects/nzmqt
NZMQT_ANDROID_DIR = $$NZMQTDIR/bin/android/
ZEROMQ_ANDROID_DIR = /opt/zeromq-android
PROTOBUF_ANDROID_DIR = /home/alexander/projects/tmp/protobuf-2.5.0
NZMQT_X86_DIR = $$NZMQTDIR/bin/

# Add more folders to ship with the application, here
folder_01.source = qml/QtQuickVcp
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    hal/qpin.cpp \
    hal/qcomponent.cpp \
    hal/qservicediscovery.cpp \
    hal/qservice.cpp
#    $$LINUXCNCDIR/src/middleware/service-discovery/sdiscover.cc

HEADERS += \
    hal/qpin.h \
    hal/qcomponent.h \
    hal/qservicediscovery.h \
    hal/qservice.h
#    $$LINUXCNCDIR/src/middleware/service-discovery/sdiscover.h

INCLUDEPATH += \
    $$NZMQTDIR/include \
    $$NZMQTDIR/externals/include \
    $$LINUXCNCDIR/src/middleware/service-discovery

LIBS += -lzmq -lprotobuf -lnzmqt

android: {
LIBS += -L$$ZEROMQ_ANDROID_DIR/lib/ \
        -L$$NZMQT_ANDROID_DIR \
        -L$$PROTOBUF_ANDROID_DIR/src/.libs/
INCLUDEPATH += $$ZEROMQ_ANDROID_DIR/include/ \
            $$PROTOBUF_ANDROID_DIR/src/
}
else: {
LIBS += -L$$NZMQT_X86_DIR
}

# Installation path
# target.path =

#Protobuf
include(protobuf.pri)
PROTOS_IN += $$LINUXCNCDIR/src/middleware/proto/types.proto \
          $$LINUXCNCDIR/src/middleware/proto/emcclass.proto \
          $$LINUXCNCDIR/src/middleware/proto/value.proto \
          $$LINUXCNCDIR/src/middleware/proto/motcmds.proto \
#          $$LINUXCNCDIR/src/middleware/nanopb/generator/proto/nanopb.proto \
          $$LINUXCNCDIR/src/middleware/proto/canon.proto \
          $$LINUXCNCDIR/src/middleware/proto/halrcomp.proto \
          $$LINUXCNCDIR/src/middleware/proto/halupdate.proto \
          $$LINUXCNCDIR/src/middleware/proto/log.proto \
          $$LINUXCNCDIR/src/middleware/proto/object.proto \
          $$LINUXCNCDIR/src/middleware/proto/rtapi_message.proto \
          $$LINUXCNCDIR/src/middleware/proto/rtapicommand.proto \
          $$LINUXCNCDIR/src/middleware/proto/rtmessage.proto \
          $$LINUXCNCDIR/src/middleware/proto/task.proto \
          $$LINUXCNCDIR/src/middleware/proto/test.proto \
          $$LINUXCNCDIR/src/middleware/proto/message.proto

# Please do not modify the following two lines. Required for deployment.
include(qtquick2controlsapplicationviewer/qtquick2controlsapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qml/QtQuickVcp/StartPage.qml \
    qml/QtQuickVcp/ViewPage.qml
