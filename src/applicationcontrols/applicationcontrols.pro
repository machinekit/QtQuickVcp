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
    EstopAction.qml \
    PowerAction.qml \
    OpenAction.qml \
    ReopenAction.qml \
    RunProgramAction.qml \
    StepProgramAction.qml \
    StopProgramAction.qml \
    PauseResumeProgramAction.qml \
    ApplicationNotifications.qml \
    ApplicationProgressBar.qml \
    ApplicationFileDialog.qml \
    JogStick.qml \
    DigitalReadOut.qml \
    FeedrateSlider.qml \
    SpindlerateSlider.qml \
    MdiCommandAction.qml \
    JogAction.qml \
    JogButton.qml \
    JogDistanceComboBox.qml \
    JogDistanceHandler.qml \
    SpindleCwAction.qml \
    SpindleCcwAction.qml \
    StopSpindleAction.qml \
    IncreaseSpindleSpeedAction.qml \
    DecreaseSpindleSpeedAction.qml \
    HomeAxisAction.qml \
    MistAction.qml \
    FloodAction.qml \
    MaximumVelocitySlider.qml \
    MaximumVelocityHandler.qml \
    TouchOffAction.qml \
    TouchOffDialog.qml \
    GCodeLabel.qml \
    ConnectionWindow.qml \
    ServiceWindow.qml \
    JogVelocitySlider.qml \
    JogVelocityHandler.qml \
    SpindlerateHandler.qml \
    FeedrateHandler.qml \
    UnhomeAxisAction.qml \
    MdiHistoryTable.qml \
    MdiCommandEdit.qml \
    AxisRadioGroup.qml \
    AxisComboBox.qml \
    TeleopAction.qml \
    ShutdownAction.qml \
    UserCommandAction.qml \
    ApplicationRemoteFileDialog.qml \
    OverrideLimitsAction.qml \
    OptionalStopAction.qml \
    BlockDeleteAction.qml \
    FeedHoldAction.qml \
    FeedOverrideAction.qml

QML_FILES += $$CONTROLS_QML_FILES

include(Private/private.pri)

QML_INFRA_FILES += \
    $$CONTROLS_QML_FILES \
    qmldir

QML_DESIGNER_FILES = \
    designer

QML_PROPERTY_EDITOR_FILES = 

include(../deployment.pri)
