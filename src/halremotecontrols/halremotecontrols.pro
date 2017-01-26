TEMPLATE = lib
QT += qml quick

release: {
    CONFIG += qtquickcompiler
}

uri = Machinekit.HalRemote.Controls
include(../plugin.pri)

# Input
SOURCES += \
    plugin.cpp

HEADERS += \
    plugin.h

RESOURCES += \
    halremotecontrols.qrc

QML_FILES = \
    $$PWD/HalApplicationWindow.qml \
    $$PWD/HalButton.qml \
    $$PWD/HalCheckBox.qml \
    $$PWD/HalColorPicker.qml \
    $$PWD/HalComboBox.qml \
    $$PWD/HalContainer.qml \
    $$PWD/HalDial.qml \
    $$PWD/HalGauge.qml \
    $$PWD/HalKnob.qml \
    $$PWD/HalLabel.qml \
    $$PWD/HalLed.qml \
    $$PWD/HalLogChart.qml \
    $$PWD/HalProgressBar.qml \
    $$PWD/HalRadioButton.qml \
    $$PWD/HalRoundGauge.qml \
    $$PWD/HalSlider.qml \
    $$PWD/HalSpinBox.qml \
    $$PWD/HalSwitch.qml \
    $$PWD/HalVirtualJoystick.qml

QML_INFRA_FILES = \
    $$QML_FILES \
    qmldir

QML_DESIGNER_FILES = \
    designer

QML_PROPERTY_EDITOR_FILES = \
    propertyEditorQmlSources/HalGaugeSpecifics.qml \
    propertyEditorQmlSources/HalSection.qml \
    propertyEditorQmlSources/ButtonSection.qml \
    propertyEditorQmlSources/HalButtonSpecifics.qml \
    propertyEditorQmlSources/CheckBoxSection.qml \
    propertyEditorQmlSources/HalCheckBoxSpecifics.qml \
    propertyEditorQmlSources/HalColorPickerSpecifics.qml \
    propertyEditorQmlSources/ComboBoxSection.qml \
    propertyEditorQmlSources/HalComboBoxSpecifics.qml \
    propertyEditorQmlSources/HalContainerSpecifics.qml \
    propertyEditorQmlSources/LabelSection.qml \
    propertyEditorQmlSources/HalLabelSpecifics.qml \
    propertyEditorQmlSources/HalLedSpecifics.qml \
    propertyEditorQmlSources/HalLogChartSpecifics.qml \
    propertyEditorQmlSources/ProgressBarSection.qml \
    propertyEditorQmlSources/HalProgressBarSpecifics.qml \
    propertyEditorQmlSources/RadioButtonSection.qml \
    propertyEditorQmlSources/HalRadioButtonSpecifics.qml \
    propertyEditorQmlSources/HalRoundGaugeSpecifics.qml \
    propertyEditorQmlSources/SliderSection.qml \
    propertyEditorQmlSources/HalSliderSpecifics.qml \
    propertyEditorQmlSources/SwitchSection.qml \
    propertyEditorQmlSources/HalSwitchSpecifics.qml \
    propertyEditorQmlSources/SpinBoxSection.qml \
    propertyEditorQmlSources/HalSpinBoxSpecifics.qml \
    propertyEditorQmlSources/HalVirtualJoystickSpecifics.qml

include(../deployment.pri)

DISTFILES += \
    designer/halremotecontrols.metainfo

TRANSLATIONS_PATH = $$PWD/translations
include(../translation.pri)
