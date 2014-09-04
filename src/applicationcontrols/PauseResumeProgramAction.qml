import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

Action {
    property var status: {"synced": false}
    property var command: {"connected": false}
    property string pauseShortcut: "P"
    property string resumeShortcut: "S"

    property bool _ready: status.synced && command.connected

    id: root
    checkable: true
    text: checked ? qsTr("Resume") : qsTr("Pause")
    iconSource: "qrc:Machinekit/Application/Controls/icons/go-pause"
    shortcut: checked ? resumeShortcut : pauseShortcut
    tooltip: (checked ? qsTr("Pause execution") : qsTr("Resume execution")) + " [" + shortcut + "]"
    onTriggered: {
        if (status.task.taskMode !== ApplicationStatus.TaskModeAuto)
            command.setTaskMode(ApplicationCommand.TaskModeAuto)
        if (checked) {
            command.pauseProgram();
        } else {
            command.resumeProgram();
        }
    }
    enabled: _ready
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && status.running
}
