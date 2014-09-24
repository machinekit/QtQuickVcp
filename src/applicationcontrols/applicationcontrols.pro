TEMPLATE = lib
QT += qml quick

uri = Machinekit.Application.Controls
include(../plugin.pri)

# Input
SOURCES += \
    plugin.cpp \
    dummy.cpp

HEADERS += \
    plugin.h \
    dummy.h

RESOURCES += \
    applicationcontrols.qrc

QML_FILES = \
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
    MdiCommandEdit.qml

QML_INFRA_FILES = \
    $$QML_FILES \
    qmldir

#QML_DESIGNER_FILES = \
#    designer

QML_PROPERTY_EDITOR_FILES = 

include(../deployment.pri)
