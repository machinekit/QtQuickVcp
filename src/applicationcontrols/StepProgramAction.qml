import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

Action {
    property var status
    property var command

    id: root
    text: qsTr("Step")
    iconSource: "qrc:Machinekit/Application/Controls/icons/go-next"
    shortcut: "T"
    tooltip: qsTr("Execute next line") + " [" + shortcut + "]"
    onTriggered: {
        if (status.task.taskMode !== ApplicationStatus.TaskModeAuto)
            command.setTaskMode(ApplicationCommand.TaskModeAuto)
        command.stepProgram()
    }
    enabled: (status !== undefined)
             && (command !== undefined)
             && (status.connectionState === ApplicationStatus.Connected)
             && (command.connectionState === ApplicationCommand.Connected)
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && (status.task.file !== "")
             && !status.running
}
