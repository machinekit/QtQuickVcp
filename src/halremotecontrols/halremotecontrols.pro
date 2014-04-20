TEMPLATE = lib
TARGET = machinekithalremotecontrolsplugin
TARGETPATH = Machinekit/HalRemote/Controls
QT += qml quick
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = Machinekit.HalRemote.Controls

# Input
SOURCES += \
    plugin.cpp

HEADERS += \
    plugin.h

RESOURCES += \
    halremotecontrols.qrc

QML_FILES = \
    HalApplicationWindow.qml \
    HalButton.qml \
    HalColorPicker.qml \
    HalDiscoveryPage.qml \
    HalGauge.qml \
    HalLed.qml \
    HalLogChart.qml \
    HalProgressBar.qml \
    HalRadioButton.qml \
    HalSlider.qml \
    HalToggleButton.qml \
    HalVirtualJoystick.qml

QML_INFRA_FILES += \
    $$QML_FILES \
    qmldir

include(../deployment.pri)
