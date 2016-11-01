TEMPLATE = lib
QT += qml quick network

uri = Machinekit.PathView
include(../plugin.pri)

include(../zeromq.pri)
include(../../3rdparty/machinetalk-protobuf-qt/machinetalk-protobuf-lib.pri)
include(../common/common.pri)

# Input
SOURCES += \
    plugin.cpp \
    qglcamera.cpp \
    gcodeprogramitem.cpp \
    gcodeprogramloader.cpp \
    gcodeprogrammodel.cpp \
    glcanvas.cpp \
    glcubeitem.cpp \
    glcylinderitem.cpp \
    glitem.cpp \
    glpathitem.cpp \
    gllight.cpp \
    glsphereitem.cpp \
    glview.cpp \
    previewclient.cpp

HEADERS += \
    plugin.h \
    debughelper.h \
    gcodeprogramitem.h \
    gcodeprogramloader.h \
    gcodeprogrammodel.h \
    glcanvas.h \
    glcubeitem.h \
    glcylinderitem.h \
    qglcamera.h \
    glitem.h \
    gllight.h \
    glpathitem.h \
    glsphereitem.h \
    glview.h \
    previewclient.h

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
    PathViewObject.qml \
    ProgramExtents3D.qml \
    ProgramOffsets3D.qml \
    SourceView.qml \
    ViewModeAction.qml \
    ZoomInAction.qml \
    ZoomOutAction.qml \
    ZoomOriginalAction.qml

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
