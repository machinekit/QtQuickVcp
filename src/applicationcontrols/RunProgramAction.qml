import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

Action {
    property var status
    property var command
    property int programStartLine: 0
    property int programResetLine: 0

    id: root
    text: qsTr("Run")
    iconSource: "qrc:Machinekit/Application/Controls/icons/go-next-view"
    shortcut: "R"
    tooltip: qsTr("Begin executing current file") + " [" + shortcut + "]"
    onTriggered: {
        if (status.task.taskMode !== ApplicationStatus.TaskModeAuto)
            command.setTaskMode(ApplicationCommand.TaskModeAuto)
        command.runProgram(programStartLine)
        programStartLine = programResetLine
    }
    enabled: (status !== undefined)
             && (command !== undefined)
             && (status.connectionState === ApplicationStatus.Connected)
             && (command.connectionState === ApplicationCommand.Connected)
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && (status.task.file !== "")
             && !status.running
}
