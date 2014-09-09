import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

Action {
    property var status: {"synced": false}
    property var command: {"connected": false}
    property var file: {"ready": false}

    property bool _ready: status.synced && command.connected && file.ready

    id: openAction
    text: qsTr("Reopen file")
    //iconName: "document-open"
    iconSource: "qrc:Machinekit/Application/Controls/icons/view-refresh"
    shortcut: "Ctrl+R"
    tooltip: qsTr("Reopen curren file") + " [" + shortcut + "]"
    onTriggered: {
        command.resetProgram('execute')
        command.openProgram('execute', file.fileName)
    }
    enabled: _ready
             && (status.task.file !== "")
}
