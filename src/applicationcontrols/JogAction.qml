import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

ApplicationAction {
    property int axis: 0
    property double velocity: 0.0
    property double distance: 0.0

    property bool _ready: status.synced && command.connected

    id: root
    text: ""
    shortcut: ""
    tooltip: qsTr("Jog Axis ") + axis + " [" + shortcut + "]"
    onTriggered: {
        if (status.task.taskMode !== ApplicationStatus.TaskModeManual) {
            command.setTaskMode('execute', ApplicationCommand.TaskModeManual)
        }
        if (velocity !== 0.0) {
            if (distance == 0.0) {
                command.jog(ApplicationCommand.ContinousJog, axis, velocity)
            }
            else {
                command.jog(ApplicationCommand.IncrementJog, axis, velocity, distance)
            }
        } else {
            command.jog(ApplicationCommand.StopJog, axis)
        }
    }

    enabled: _ready
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && !status.running
}
