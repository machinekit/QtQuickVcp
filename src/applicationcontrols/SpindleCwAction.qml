import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

Action {
    property var status: {"synced": false}
    property var command: {"connected": false}

    property bool _ready: status.synced && command.connected

    id: root
    text: qsTr("CW")
    shortcut: "F9"
    tooltip: qsTr("Turn spindle clockwise") + " [" + shortcut + "]"
    onTriggered: {
        if (status.task.taskMode !== ApplicationStatus.TaskModeManual) {
            command.setTaskMode(ApplicationCommand.TaskModeManual)
        }
        command.setSpindle(ApplicationCommand.SpindleForward, status.config.defaultSpindleSpeed)
    }

    checkable: true
    checked: _ready ? (status.motion.spindleDirection === 1) : false
    enabled: _ready
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && !status.running
}

