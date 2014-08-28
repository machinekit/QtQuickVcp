import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

Action {
    property var status
    property var command
    property string pauseShortcut: "P"
    property string resumeShortcut: "S"

    id: root
    checkable: true
    text: checked ? qsTr("Resume") : qsTr("Pause")
    iconSource: "qrc:Machinekit/Application/Controls/icons/go-pause"
    shortcut: checked ? resumeShortcut : pauseShortcut
    tooltip: (checked ? qsTr("Pause execution") : qsTr("Resume execution")) + " [" + shortcut + "]"
    onTriggered: {
        if (status.task.taskMode !== ApplicationStatus.TaskModeAuto)
            command.setTaskMode(ApplicationCommand.TaskModeAuto)
        if (checked) {
            command.pauseProgram();
        } else {
            command.resumeProgram();
        }
    }
    enabled: (status !== undefined)
             && (command !== undefined)
             && (status.connectionState === ApplicationStatus.Connected)
             && (command.connectionState === ApplicationCommand.Connected)
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && status.running
}
