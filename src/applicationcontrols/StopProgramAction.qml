import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

Action {
    property var status
    property var command

    id: root
    text: qsTr("Stop")
    iconSource: "qrc:Machinekit/Application/Controls/icons/go-stop"
    shortcut: "Esc"
    tooltip: qsTr("Stop program execution") + " [" + shortcut + "]"
    onTriggered: {
        command.abort()
    }
    enabled: (status !== undefined)
             && (command !== undefined)
             && (status.connectionState === ApplicationStatus.Connected)
             && (command.connectionState === ApplicationCommand.Connected)
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && status.running
}
