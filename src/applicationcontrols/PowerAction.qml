import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

Action {
    property var status: {"synced": false}
    property var command: {"connected": false}

    property bool _ready: status.synced && command.connected

    id: root
    text: qsTr("Power")
    //iconName: "system-shutdown"
    iconSource: "qrc:Machinekit/Application/Controls/icons/system-shutdown"
    shortcut: "F2"
    tooltip: qsTr("Toggle Machine power") + " [" + shortcut + "]"
    checkable: true
    onTriggered: {
        if (checked) {
            command.setTaskState(ApplicationCommand.TaskStateOn)
        }
        else {
            command.setTaskState(ApplicationCommand.TaskStateOff)
        }
    }

    checked: _ready && (status.task.taskState === ApplicationStatus.TaskStateOn)
    enabled: _ready && (status.task.taskState !== ApplicationStatus.TaskStateEstop)
}
