# Modify these paths to your needs and then build the project

# Android paths
ZEROMQ_ANDROID_DIR = /opt/zeromq-android
PROTOBUF_ANDROID_DIR = /opt/protobuf-android

# Mac paths
ZEROMQ_MAC_DIR = /Users/strahlex/libzmq-ios/libzmq_dist
PROTOBUF_MAC_DIR = /opt/local

# Here should come the new better path management
android: {
    ZEROMQ_INCLUDE_PATH = /opt/zeromq-android/include
    ZEROMQ_LIB_PATH = /opt/zeromq-android/lib
}

# Qt SDK directory - autodetected based on qmake path
QTSDK_DIR=$$absolute_path($$[QT_INSTALL_PREFIX]/../..)

# Qt Creator paths for the Qt Quick Designer plugin
# Path to the Qt Creator source - not needed anymore
# QTCREATOR_SOURCE_DIR=/home/alexander/bin/qt-creator
# Path to installed Qt Creator (where the plugin should be installed to)
QTCREATOR_INSTALL_DIR=$$QTSDK_DIR/Tools/QtCreator

# Qt documentation directory containing Qt documentation with .index files
# -> somehow only the Android toolchain comes with .index files
QT_DOC_DIR = $$[QT_INSTALL_PREFIX]/../android_armv7/doc
