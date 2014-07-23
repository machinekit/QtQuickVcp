TEMPLATE = lib
TARGET = machinekitpathviewplugin
TARGETPATH = Machinekit/PathView
QT += qml quick
CONFIG += qt plugin
CONFIG -= android_install

TARGET = $$qtLibraryTarget($$TARGET)
uri = Machinekit.PathView
QMAKE_MOC_OPTIONS += -Muri=$$uri

# Input
SOURCES += \
    plugin.cpp \
    qglview.cpp \
    qpathview3d.cpp \
    qglitem.cpp \
    qglcubeitem.cpp \
    qglgriditem.cpp \
    qglsphereitem.cpp \
    qglcylinderitem.cpp \
    qglcamera.cpp \
    qglcoordinateitem.cpp

HEADERS += \
    plugin.h \
    qglview.h \
    qpathview3d.h \
    qglitem.h \
    qglcubeitem.h \
    qglgriditem.h \
    qglsphereitem.h \
    qglcylinderitem.h \
    qglcamera.h \
    qglcoordinateitem.h

QML_INFRA_FILES = \
    qmldir

include(../deployment.pri)

OTHER_FILES += \
    SimpleVertex.glsl \
    SimpleFragment.glsl \
    LineVertexShader.glsl \
    LineFragmentShader.glsl \
    TextFragmentShader.glsl \
    TextVertexShader.glsl

RESOURCES += \
    shaders.qrc

