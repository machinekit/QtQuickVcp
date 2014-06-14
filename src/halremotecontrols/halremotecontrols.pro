TEMPLATE = lib
TARGET = machinekithalremotecontrolsplugin
TARGETPATH = Machinekit/HalRemote/Controls
QT += qml quick
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = Machinekit.HalRemote.Controls
QMAKE_MOC_OPTIONS += -Muri=$$uri

# Input
SOURCES += \
    plugin.cpp \
    dummy.cpp

HEADERS += \
    plugin.h \
    dummy.h

RESOURCES += \
    halremotecontrols.qrc

QML_FILES = \
    ConnectionWindow.qml \
    HalApplicationWindow.qml \
    HalButton.qml \
    HalCheckBox.qml \
    HalColorPicker.qml \
    HalComboBox.qml \
    HalContainer.qml \
    HalDial.qml \
    HalGauge.qml \
    HalLabel.qml \
    HalLed.qml \
    HalLogChart.qml \
    HalProgressBar.qml \
    HalRadioButton.qml \
    HalRoundGauge.qml \
    HalSlider.qml \
    HalSpinBox.qml \
    HalVirtualJoystick.qml

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
    propertyEditorQmlSources/SliderSection.qml \
    propertyEditorQmlSources/HalSliderSpecifics.qml \
    propertyEditorQmlSources/SpinBoxSection.qml \
    propertyEditorQmlSources/HalSpinBoxSpecifics.qml \
    propertyEditorQmlSources/HalVirtualJoystickSpecifics.qml

include(../deployment.pri)
