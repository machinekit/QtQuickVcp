TEMPLATE = app
TARGET = machinekit-client

QT += qml quick widgets
CONFIG += c++11

release: {
    CONFIG += qtquickcompiler
}

SOURCES += main.cpp

RESOURCES += \
    qml.qrc \
    icons.qrc

QML_FILES = $$PWD/init.qml \
    $$PWD/main.qml \
    $$PWD/SplashScreen.qml \
    $$PWD/MachinekitClient.ServiceDisplay/ServiceDisplay.qml
OTHER_FILES += $$QML_FILES

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# deactivate installs for applications
INSTALLS =

ios: {
    LIBS += -L$$PROTOBUF_LIB_PATH
    LIBS += -L$$ZEROMQ_LIB_PATH
    LIBS += -L$$LIBSODIUM_LIB_PATH
    LIBS += -lsodium -lzmq -lprotobuf
    LIBS += -lmachinetalk-pb2
    QMAKE_INFO_PLIST = Info.plist
    QMAKE_POST_LINK += cp -n $$PWD/icons/machinekit2.iconset/* $${OUT_PWD}/.
}

android: {
    include(../../3rdparty/machinetalk-protobuf-qt/machinetalk-protobuf-lib.pri)
    QML_IMPORT_PATH = $$OUT_PWD/../../imports
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
    mac_Info.plist \
    misc/*.*

NAME = ServiceDisplay
TRANSLATIONS_PATH = $$PWD/translations
TRANSLATIONS_OUT_PATH = $$PWD/MachinekitClient.$${NAME}/translations
include(translation.pri)

linux: !android: {
target.path = /usr/bin

desktop.path = /usr/share/applications
desktop.files = misc/machinekit-client.desktop

icon.path = /usr/share/pixmaps
icon.files = icons/machinekit.png

INSTALLS += target desktop icon
}
