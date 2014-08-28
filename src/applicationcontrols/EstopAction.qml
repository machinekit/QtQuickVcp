import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

Action {
    property var status

    id: root
    text: qsTr("Estop")
    //iconName: "process-stop"
    iconSource: "qrc:Machinekit/Application/Controls/icons/process-stop"
    shortcut: "F1"
    tooltip: qsTr("Toggle Emergency Stop") + " [" + shortcut + "]"
    checkable: true
    onTriggered: {
        if (checked) {
            command.setTaskState(ApplicationCommand.TaskStateEstop)
        }
        else {
            command.setTaskState(ApplicationCommand.TaskStateEstopReset)
        }
    }

    checked: (status !== undefined) && (status.task.taskState !== undefined) && (status.task.taskState === ApplicationStatus.TaskStateEstop)
    enabled: (status !== undefined) && (status.task.taskState !== undefined)
}
