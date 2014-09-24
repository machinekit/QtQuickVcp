import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

ApplicationAction {
    property var touchOffDialog

    property bool _ready: status.synced && command.connected

    id: root
    text: qsTr("Touch Off")
    shortcut: "End"
    tooltip: qsTr("Set G5x offset for active axis") + " [" + shortcut + "]"
    onTriggered: {
        touchOffDialog.visible = true
    }

    enabled: _ready
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && !status.running
}

