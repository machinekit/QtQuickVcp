TEMPLATE = lib
QT += qml quick

uri = Machinekit.PathView
include(../plugin.pri)

include(../zeromq.pri)
include(../../3rdparty/machinetalk-protobuf-qt/machinetalk-protobuf-lib.pri)

# Input
SOURCES += \
    plugin.cpp \
    qglview.cpp \
    qglitem.cpp \
    qglcubeitem.cpp \
    qglsphereitem.cpp \
    qglcylinderitem.cpp \
    qglcamera.cpp \
    qgllight.cpp \
    qglpathitem.cpp \
    qglcanvas.cpp \
    qpreviewclient.cpp \
    qgcodeprogramitem.cpp \
    qgcodeprogrammodel.cpp \
    qgcodeprogramloader.cpp

HEADERS += \
    plugin.h \
    qglview.h \
    qglitem.h \
    qglcubeitem.h \
    qglsphereitem.h \
    qglcylinderitem.h \
    qglcamera.h \
    qgllight.h \
    qglpathitem.h \
    qglcanvas.h \
    qpreviewclient.h \
    debughelper.h \
    qgcodeprogramitem.h \
    qgcodeprogrammodel.h \
    qgcodeprogramloader.h

RESOURCES += \
    shaders.qrc \
    pathview.qrc

QML_FILES = \
    BoundingBox3D.qml \
    Coordinate3D.qml \
    GCodeSync.qml \
    Grid3D.qml \
    PathView3D.qml \
    PathViewCore.qml \
    ProgramExtents3D.qml \
    SourceView.qml

QML_INFRA_FILES = \
    $$QML_FILES \
    qmldir

OTHER_FILES += \
    SimpleVertex.glsl \
    SimpleFragment.glsl \
    LineVertexShader.glsl \
    LineFragmentShader.glsl \
    TextFragmentShader.glsl \
    TextVertexShader.glsl

include(../deployment.pri)
