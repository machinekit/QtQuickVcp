TEMPLATE = lib
TARGET = machinekitcontrolsplugin
TARGETPATH = Machinekit/Controls
QT += qml quick
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = Machinekit.Controls
QMAKE_MOC_OPTIONS += -Muri=$$uri

# Input
SOURCES += \
    plugin.cpp \
    dummy.cpp

HEADERS += \
    plugin.h \
    dummy.h

RESOURCES += \
    controls.qrc

QML_FILES = \
    Gauge.qml \
    ColorPicker.qml \
    Led.qml \
    Line.qml \
    LogChart.qml \
    TemperatureSelector.qml \
    TouchButton.qml \
    ValueChart.qml \
    ValueModel.qml \
    VirtualJoystick.qml

QML_INFRA_FILES = \
    $$QML_FILES \
    qmldir

QML_DESIGNER_FILES = \
    designer

QML_PROPERTY_EDITOR_FILES = \
    propertyEditorQmlSources/GaugeSpecifics.qml \
    propertyEditorQmlSources/GaugeSection.qml \
    propertyEditorQmlSources/TextSection.qml \
    propertyEditorQmlSources/ColorPickerSection.qml \
    propertyEditorQmlSources/ColorPickerSpecifics.qml \
    propertyEditorQmlSources/LedSpecifics.qml \
    propertyEditorQmlSources/LedSection.qml \
    propertyEditorQmlSources/LogChartSection.qml \
    propertyEditorQmlSources/LogChartSpecifics.qml \
    propertyEditorQmlSources/ValueChartSection.qml \
    propertyEditorQmlSources/ValueChartSpecifics.qml \
    propertyEditorQmlSources/VirtualJoystickSection.qml \
    propertyEditorQmlSources/VirtualJoystickSpecifics.qml

include(../deployment.pri)
