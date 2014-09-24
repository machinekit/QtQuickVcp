import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

ApplicationAction {
    property bool _ready: status.synced && command.connected

    id: root
    text: qsTr("Flood")
    shortcut: ""
    tooltip: qsTr("Enable flood") + " [" + shortcut + "]"
    onTriggered: {
        if (status.task.taskMode !== ApplicationStatus.TaskModeManual)
            command.setTaskMode('execute', ApplicationCommand.TaskModeManual)
        command.setFloodEnabled(checked)
    }

    checkable: true

    checked: _ready ? status.io.flood : false

    enabled: _ready
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && !status.running
}

