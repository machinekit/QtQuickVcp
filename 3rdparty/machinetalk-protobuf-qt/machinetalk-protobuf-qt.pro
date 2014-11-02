QT     -= core gui
CONFIG -= android_install
CONFIG += release

TARGET = machinetalk-protobuf
TEMPLATE = lib

include(../../paths.pri)
include(machinetalk-protobuf.pri)

win32: target.path = $$[QT_INSTALL_BINS]
!win32: target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target
