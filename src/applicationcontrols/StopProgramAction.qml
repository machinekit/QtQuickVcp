import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

ApplicationAction {
    property bool _ready: status.synced && command.connected

    id: root
    text: qsTr("Stop")
    iconSource: "qrc:Machinekit/Application/Controls/icons/go-stop"
    shortcut: "Esc"
    tooltip: qsTr("Stop program execution") + " [" + shortcut + "]"
    onTriggered: {
        command.abort('execute')
    }
    enabled: _ready
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && status.running
}
