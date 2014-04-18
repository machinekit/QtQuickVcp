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
    ApplicationWindow.qml \
    Button.qml \
    ColorPicker.qml \
    DiscoveryPage.qml \
    Gauge.qml \
    Led.qml \
    LogChart.qml \
    ProgressBar.qml \
    RadioButton.qml \
    Slider.qml \
    ToggleButton.qml \
    VirtualJoystick.qml

QML_INFRA_FILES += \
    $$QML_FILES \
    qmldir

include(../deployment.pri)
#DEPLOYMENT -= target
#DESTDIR =
