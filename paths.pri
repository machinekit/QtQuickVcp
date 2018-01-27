# Modify these paths to your needs and then build the project

# Add here the paths that are not in the global PATH or INCLUDEPATH
# environment variables usually for mobile OS
android: {
    ZEROMQ_INCLUDE_PATH = /opt/zeromq-android/include
    ZEROMQ_LIB_PATH = /opt/zeromq-android/lib
    ZEROMQ_LIB_FLAGS = -Bstatic
    PROTOBUF_INCLUDE_PATH = /opt/protobuf-android/include
    PROTOBUF_LIB_PATH = /opt/protobuf-android/lib
    PROTOBUF_LIB_FLAGS = -Bstatic
}
ios: {
    ZEROMQ_INCLUDE_PATH = /opt/zeromq-ios/include
    ZEROMQ_LIB_PATH = /opt/zeromq-ios/lib
    ZEROMQ_LIB_FLAGS = -Bstatic
    PROTOBUF_INCLUDE_PATH = /opt/protobuf-ios/include
    PROTOBUF_LIB_PATH = /opt/protobuf-ios/lib
    PROTOBUF_LIB_FLAGS = -Bstatic
    PROTOBUF_PROTOC = /opt/protobuf-ios/bin/protoc
    LIBSODIUM_LIB_PATH = /opt/libsodium-ios/lib
}
macx: {
    ZEROMQ_INCLUDE_PATH = /opt/local/include
    ZEROMQ_LIB_PATH = /opt/local/lib
    ZEROMQ_LIB_FLAGS = -Bstatic
    PROTOBUF_INCLUDE_PATH = /opt/local/include
    PROTOBUF_LIB_PATH = /opt/local/lib
    PROTOBUF_PROTOC = /opt/local/bin/protoc
    PROTOBUF_LIB_FLAGS = -Bstatic
}
win32-msvc*: {
    CONFIG(debug, debug|release): PROTOBUF_DEBUG = Debug
    CONFIG(release, debug|release): PROTOBUF_DEBUG = Release

    PROTOBUF_INCLUDE_PATH = $$(HOMEDRIVE)$$(HOMEPATH)/bin/protobuf/src
    ZEROMQ_INCLUDE_PATH = $$(HOMEDRIVE)$$(HOMEPATH)/bin/zeromq4-x/include
    contains(QMAKE_TARGET.arch, x86_64) {
        ZEROMQ_LIB_PATH = $$(HOMEDRIVE)$$(HOMEPATH)/bin/zeromq4-x/lib/x64
        PROTOBUF_LIB_PATH = $$(HOMEDRIVE)$$(HOMEPATH)/bin/protobuf/vsprojects/x64/$$PROTOBUF_DEBUG
        PROTOBUF_PROTOC = $$(HOMEDRIVE)$$(HOMEPATH)/bin/protobuf/vsprojects/x64/$$PROTOBUF_DEBUG/protoc.exe
    }
    else {
        ZEROMQ_LIB_PATH = $$(HOMEDRIVE)$$(HOMEPATH)/bin/zeromq4-x/lib\Win32
        PROTOBUF_LIB_PATH = $$(HOMEDRIVE)$$(HOMEPATH)/bin/protobuf/vsprojects/$$PROTOBUF_DEBUG
        PROTOBUF_PROTOC = $$(HOMEDRIVE)$$(HOMEPATH)/bin/protobuf/vsprojects/$$PROTOBUF_DEBUG/protoc.exe
    }
}
win32-g++ {
    ZEROMQ_INCLUDE_PATH = $$(HOMEDRIVE)$$(HOMEPATH)\bin\MinGW\include
    ZEROMQ_LIB_PATH = $$(HOMEDRIVE)$$(HOMEPATH)\bin\MinGW\lib
    PROTOBUF_INCLUDE_PATH =  $$(HOMEDRIVE)$$(HOMEPATH)\bin\MinGW\include
    PROTOBUF_LIB_PATH =  $$(HOMEDRIVE)$$(HOMEPATH)\bin\MinGW\lib
    PROTOBUF_PROTOC =  $$(HOMEDRIVE)$$(HOMEPATH)\bin\MinGW\bin\protoc.exe
}
linux: !android: {
	equals(QMAKE_CXX, g++) \
	: system(g++ -dumpversion | grep ^6 ) {
		PROTOBUF_INCLUDE_PATH =
	} else {
		PROTOBUF_INCLUDE_PATH = /usr/include
	}
    linux-clang: {
        PROTOBUF_INCLUDE_PATH =
    }
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
# install one of the Android toolchains to work properly
QT_DOC_DIR = $$[QT_INSTALL_DOCS]
