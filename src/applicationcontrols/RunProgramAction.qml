import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

ApplicationAction {
    property int programStartLine: 0
    property int programResetLine: 0

    property bool _ready: status.synced && command.connected

    id: root
    text: qsTr("Run")
    iconSource: "qrc:Machinekit/Application/Controls/icons/go-next-view"
    shortcut: "R"
    tooltip: qsTr("Begin executing current file") + " [" + shortcut + "]"
    onTriggered: {
        if (status.task.taskMode !== ApplicationStatus.TaskModeAuto)
            command.setTaskMode('execute', ApplicationCommand.TaskModeAuto)
        command.runProgram('execute', programStartLine)
        programStartLine = programResetLine
    }
    enabled: _ready
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && (status.task.file !== "")
             && !status.running
}
