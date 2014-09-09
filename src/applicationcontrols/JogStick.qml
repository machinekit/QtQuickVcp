import QtQuick 2.0
import Machinekit.Controls 1.0
import Machinekit.Application 1.0

VirtualJoystick {
    property var status: {"synced": false}
    property var command: {"connected": false}
    property int xAxis: 0
    property int yAxis: 1

    property bool _ready: status.synced && command.connected

    id: root

    function _jog(axis, value) {
        if (axis === -1)
            return

        if (status.task.taskMode !== ApplicationStatus.TaskModeManual)
            command.setTaskMode('execute', ApplicationCommand.TaskModeManual)
        if (value !== 0.0) {
            command.jog(ApplicationCommand.ContinousJog, axis, value)
        } else {
            command.jog(ApplicationCommand.StopJog, axis)
        }
    }

    enabled: _ready
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && !status.running

    onXValueChanged: _jog(xAxis, xValue)

    onYValueChanged: _jog(yAxis, yValue)
}
