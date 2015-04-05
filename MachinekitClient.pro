TEMPLATE = app

TARGET = machinekit-client

# CONFIG += console

QT += qml quick widgets

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# deactivate installs for applications
INSTALLS =

include(paths.pri)

ios: {
    LIBS += -L$$PROTOBUF_LIB_PATH
    LIBS += -L$$ZEROMQ_LIB_PATH
    LIBS += -L$$LIBSODIUM_LIB_PATH
    LIBS += -lsodium -lzmq -lprotobuf
    LIBS += -lmachinetalk-protobuf
    QMAKE_INFO_PLIST = Info.plist
    QMAKE_POST_LINK += cp -n $$PWD/icons/machinekit2.iconset/* $${OUT_PWD}/.
}

android: {
    LIBS += -lmachinetalk-protobuf
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

windows: {
    RC_FILE = icon.rc
}

macx: {
    QMAKE_INFO_PLIST = mac_Info.plist
    ICON = $$PWD/icons/machinekit.icns
    QMAKE_POST_LINK += $$QMAKE_COPY $$PWD/$${QMAKE_INFO_PLIST} $${TARGET}.app/Contents/Info.plist $$escape_expand(\n\t)
    QMAKE_POST_LINK += $$QMAKE_COPY $$ICON $${TARGET}.app/Contents/Resources/machinekit.icns
}

OTHER_FILES += \
    android/AndroidManifest.xml \
    Info.plist \
    mac_Info.plist
