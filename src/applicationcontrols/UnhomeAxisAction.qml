import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

Action {
    property var status: {"synced": false}
    property var command: {"connected": false}
    property int axis: 0

    property bool _ready: status.synced && command.connected

    id: root
    text: qsTr("Unhome")
    shortcut: "Ctrl+Shift+Home"
    tooltip: qsTr("Unhome Axis ") + axis + " [" + shortcut + "]"
    onTriggered: {
        if (status.task.taskMode !== ApplicationStatus.TaskModeManual)
            command.setTaskMode('execute', ApplicationCommand.TaskModeManual)
        command.unhomeAxis(axis)
    }

    enabled: _ready
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && !status.running
}
