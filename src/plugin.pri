CONFIG += qt plugin
CONFIG += c++14
CONFIG -= android_install
TARGET_NAME = $$lower($$replace(uri, \\., ))
TARGET = $$qtLibraryTarget($${TARGET_NAME}plugin)
QMAKE_MOC_OPTIONS += -Muri=$$uri
