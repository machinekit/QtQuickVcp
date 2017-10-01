TEMPLATE = lib
QT += qml quick

uri = Machinekit.Controls
include(../plugin.pri)

# Input
SOURCES +=

HEADERS += \
    plugin/plugin.h

SOURCES += \
    plugin/plugin.cpp

RESOURCES += \
    controls.qrc

QML_FILES = \
    $$PWD/Dial.qml \
    $$PWD/Gauge.qml \
    $$PWD/ColorPicker.qml \
    $$PWD/Knob.qml \
    $$PWD/Led.qml \
    $$PWD/LogChart.qml \
    $$PWD/RoundGauge.qml \
    $$PWD/ScaleContainer.qml \
    $$PWD/SlideView.qml \
    $$PWD/SlidePage.qml \
    $$PWD/TemperatureSelector.qml \
    $$PWD/TooltipArea.qml \
    $$PWD/TouchButton.qml \
    $$PWD/ValueChart.qml \
    $$PWD/ValueModel.qml \
    $$PWD/VirtualJoystick.qml

include(Private/private.pri)

QML_INFRA_FILES = \
    $$QML_FILES \
    $$PWD/plugin/qmldir

QML_DESIGNER_FILES = \
    $$PWD/designer

QML_PROPERTY_EDITOR_FILES = \
    propertyEditorQmlSources/DialSpecifics.qml \
    propertyEditorQmlSources/DialSection.qml \
    propertyEditorQmlSources/GaugeSpecifics.qml \
    propertyEditorQmlSources/GaugeSection.qml \
    propertyEditorQmlSources/TextSection.qml \
    propertyEditorQmlSources/ColorPickerSection.qml \
    propertyEditorQmlSources/ColorPickerSpecifics.qml \
    propertyEditorQmlSources/LedSpecifics.qml \
    propertyEditorQmlSources/LedSection.qml \
    propertyEditorQmlSources/LogChartSection.qml \
    propertyEditorQmlSources/LogChartSpecifics.qml \
    propertyEditorQmlSources/RoundGaugeSection.qml \
    propertyEditorQmlSources/RoundGaugeSpecifics.qml \
    propertyEditorQmlSources/SmallTextSection.qml \
    propertyEditorQmlSources/ValueChartSection.qml \
    propertyEditorQmlSources/ValueChartSpecifics.qml \
    propertyEditorQmlSources/VirtualJoystickSection.qml \
    propertyEditorQmlSources/VirtualJoystickSpecifics.qml

include(../deployment.pri)

TRANSLATIONS_PATH = $$PWD/translations
include(../translation.pri)
