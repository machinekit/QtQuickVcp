import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

Action {
    property var status
    property var command

    id: openAction
    text: qsTr("Reopen file")
    //iconName: "document-open"
    iconSource: "qrc:Machinekit/Application/Controls/icons/view-refresh"
    shortcut: "Ctrl+R"
    tooltip: qsTr("Reopen curren file") + " [" + shortcut + "]"
    onTriggered: {
        command.resetProgram()
    }
    enabled: (status !== undefined) && (command !== undefined) && (status.task.file !== "")
}
