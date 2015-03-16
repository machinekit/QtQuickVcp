CONFIG += qt plugin
CONFIG -= android_install
TARGET = $$qtLibraryTarget($$lower($$replace(uri, \\., ))plugin)
QMAKE_MOC_OPTIONS += -Muri=$$uri
