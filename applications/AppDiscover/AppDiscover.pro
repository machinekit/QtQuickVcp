TEMPLATE = app

TARGET = machinekit-client

CONFIG += console

QT += qml quick widgets

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# deactivate installs for applications
INSTALLS =

include(../../paths.pri)

ios: {
    LIBS += -L$$PROTOBUF_LIB_PATH
    LIBS += -L$$ZEROMQ_LIB_PATH
    LIBS += -L$$LIBSODIUM_LIB_PATH
    LIBS += -lsodium -lzmq -lprotobuf
}

android: {
    LIBS += -lmachinetalk-protobuf
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

windows {
    RC_FILE = icon.rc
}

OTHER_FILES += \
    android/AndroidManifest.xml
