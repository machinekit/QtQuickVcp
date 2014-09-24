import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

ApplicationAction {
    property bool _ready: status.synced && command.connected

    id: root
    text: qsTr("Stop")
    shortcut: "F9/F10"
    tooltip: qsTr("Stop spindle") + " [" + shortcut + "]"
    onTriggered: {
        if (status.task.taskMode !== ApplicationStatus.TaskModeManual) {
            command.setTaskMode('execute', ApplicationCommand.TaskModeManual)
        }
        command.setSpindle(ApplicationCommand.SpindleOff)
    }

    checkable: true
    checked: _ready ? (status.motion.spindleDirection === 0) : false
    enabled: _ready
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && !status.running
}

