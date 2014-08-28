import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

Action {
    property var file
    property var fileDialog

    id: openAction
    text: qsTr("Open file")
    //iconName: "document-open"
    iconSource: "qrc:Machinekit/Application/Controls/icons/document-open"
    shortcut: "O"
    tooltip: qsTr("Open G-Code file") + " [" + shortcut + "]"
    onTriggered: {
        if ((file !== undefined) && file.transferState === ApplicationFile.NoTransfer)
        {
            fileDialog.visible = true
        }
    }
    enabled: (file !== undefined) && file.ready && file.transferState === ApplicationFile.NoTransfer
}
