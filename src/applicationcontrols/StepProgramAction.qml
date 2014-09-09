import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

Action {
    property var status: {"synced": false}
    property var command: {"connected": false}

    property bool _ready: status.synced && command.connected

    id: root
    text: qsTr("Step")
    iconSource: "qrc:Machinekit/Application/Controls/icons/go-next"
    shortcut: "T"
    tooltip: qsTr("Execute next line") + " [" + shortcut + "]"
    onTriggered: {
        if (status.task.taskMode !== ApplicationStatus.TaskModeAuto)
            command.setTaskMode('execute', ApplicationCommand.TaskModeAuto)
        command.stepProgram('execute')
    }
    enabled: _ready
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && (status.task.file !== "")
             && !status.running
}
