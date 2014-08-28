TEMPLATE = lib
TARGET = machinekitapplicationcontrolsplugin
TARGETPATH = Machinekit/Application/Controls
QT += qml quick
CONFIG += qt plugin
CONFIG -= android_install

TARGET = $$qtLibraryTarget($$TARGET)
uri = Machinekit.Application.Controls
QMAKE_MOC_OPTIONS += -Muri=$$uri

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
    EstopButton.qml \
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
    ApplicationFileDialog.qml

QML_INFRA_FILES = \
    $$QML_FILES \
    qmldir

#QML_DESIGNER_FILES = \
#    designer

QML_PROPERTY_EDITOR_FILES = 

include(../deployment.pri)

OTHER_FILES +=

