TEMPLATE = lib
TARGET = machinekitcontrolsplugin
TARGETPATH = Machinekit/Controls
QT += qml quick
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = Machinekit.Controls

# Input
SOURCES += \
    plugin.cpp

HEADERS += \
    plugin.h

QML_FILES = \
    ColorPicker.qml \
    Line.qml \
    LogChart.qml \
    TemperatureSelector.qml \
    TouchButton.qml \
    ValueChart.qml \
    ValueModel.qml \
    VirtualJoystick.qml

QML_INFRA_FILES += \
    $$QML_FILES \
    qmldir
#    src/plugins.qmltypes

include(../deployment.pri)

RESOURCES += \
    controls.qrc