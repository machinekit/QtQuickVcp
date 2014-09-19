import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

Action {
    property var status: {"synced": false}
    property var command: {"connected": false}

    property bool _ready: status.synced && command.connected

    id: root
    text: qsTr("-")
    shortcut: "F11"
    tooltip: qsTr("Turn spindle slower") + " [" + shortcut + "]"
    onTriggered: {
        if (status.task.taskMode !== ApplicationStatus.TaskModeManual) {
            command.setTaskMode('execute', ApplicationCommand.TaskModeManual)
        }
        command.setSpindle(ApplicationCommand.SpindleDecrease)
    }

    enabled: _ready
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && (status.motion.spindleDirection !== 0)
             && !status.running
}

