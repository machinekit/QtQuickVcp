import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

ApplicationAction {
    property bool _ready: status.synced && command.connected

    id: root
    text: qsTr("Estop")
    //iconName: "process-stop"
    iconSource: "qrc:Machinekit/Application/Controls/icons/process-stop"
    shortcut: "F1"
    tooltip: qsTr("Toggle Emergency Stop") + " [" + shortcut + "]"
    checkable: true
    onTriggered: {
        if (checked) {
            command.setTaskState('execute', ApplicationCommand.TaskStateEstop)
        }
        else {
            command.setTaskState('execute', ApplicationCommand.TaskStateEstopReset)
        }
    }

    checked: _ready && (status.task.taskState === ApplicationStatus.TaskStateEstop)
    enabled: _ready
}
