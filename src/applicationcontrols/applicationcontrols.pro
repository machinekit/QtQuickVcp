TEMPLATE = lib
QT += qml quick

uri = Machinekit.Application.Controls
include(../plugin.pri)

# Input
SOURCES += \
    plugin.cpp

HEADERS += \
    plugin.h

RESOURCES += \
    applicationcontrols.qrc

CONTROLS_QML_FILES = \
    $$PWD/EstopAction.qml \
    $$PWD/PowerAction.qml \
    $$PWD/OpenAction.qml \
    $$PWD/ReopenAction.qml \
    $$PWD/RunProgramAction.qml \
    $$PWD/StepProgramAction.qml \
    $$PWD/StopProgramAction.qml \
    $$PWD/PauseResumeProgramAction.qml \
    $$PWD/ApplicationNotifications.qml \
    $$PWD/ApplicationProgressBar.qml \
    $$PWD/ApplicationFileDialog.qml \
    $$PWD/JogStick.qml \
    $$PWD/AbstractDigitalReadOut.qml \
    $$PWD/DigitalReadOut.qml \
    $$PWD/FeedrateSlider.qml \
    $$PWD/SpindlerateSlider.qml \
    $$PWD/MdiCommandAction.qml \
    $$PWD/JogAction.qml \
    $$PWD/JogButton.qml \
    $$PWD/JogDistanceComboBox.qml \
    $$PWD/JogDistanceHandler.qml \
    $$PWD/SpindleCwAction.qml \
    $$PWD/SpindleCcwAction.qml \
    $$PWD/StopSpindleAction.qml \
    $$PWD/IncreaseSpindleSpeedAction.qml \
    $$PWD/DecreaseSpindleSpeedAction.qml \
    $$PWD/HomeAxisAction.qml \
    $$PWD/MistAction.qml \
    $$PWD/FloodAction.qml \
    $$PWD/MaximumVelocitySlider.qml \
    $$PWD/MaximumVelocityHandler.qml \
    $$PWD/TouchOffAction.qml \
    $$PWD/TouchOffDialog.qml \
    $$PWD/GCodeLabel.qml \
    $$PWD/ConnectionWindow.qml \
    $$PWD/ServiceWindow.qml \
    $$PWD/JogVelocitySlider.qml \
    $$PWD/JogVelocityHandler.qml \
    $$PWD/SpindlerateHandler.qml \
    $$PWD/FeedrateHandler.qml \
    $$PWD/UnhomeAxisAction.qml \
    $$PWD/MdiHistoryTable.qml \
    $$PWD/MdiCommandEdit.qml \
    $$PWD/AxisRadioGroup.qml \
    $$PWD/AxisComboBox.qml \
    $$PWD/TeleopAction.qml \
    $$PWD/ShutdownAction.qml \
    $$PWD/UserCommandAction.qml \
    $$PWD/ApplicationRemoteFileDialog.qml \
    $$PWD/OverrideLimitsAction.qml \
    $$PWD/OptionalStopAction.qml \
    $$PWD/BlockDeleteAction.qml \
    $$PWD/FeedHoldAction.qml \
    $$PWD/FeedOverrideAction.qml \
    $$PWD/SpindleOverrideAction.qml \
    $$PWD/EstopPowerAction.qml \
    $$PWD/RapidrateHandler.qml \
    $$PWD/RapidrateSlider.qml \
    $$PWD/EditWithSystemEditorAction.qml

lupdate_only {
    SOURCES = $$CONTROLS_QML_FILES
}

QML_FILES += $$CONTROLS_QML_FILES

include(Private/private.pri)

QML_INFRA_FILES += \
    $$CONTROLS_QML_FILES \
    qmldir

QML_DESIGNER_FILES = \
    designer

QML_PROPERTY_EDITOR_FILES = 

include(../deployment.pri)

TRANSLATIONS_PATH = $$PWD/translations
include(../translation.pri)
