import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

ApplicationAction {
    property var fileDialog

    property bool _ready: file.ready && (file.transferState === ApplicationFile.NoTransfer)

    id: openAction
    text: qsTr("Open file")
    //iconName: "document-open"
    iconSource: "qrc:Machinekit/Application/Controls/icons/document-open"
    shortcut: "O"
    tooltip: qsTr("Open G-Code file") + " [" + shortcut + "]"
    onTriggered: {
        if (_ready)
        {
            fileDialog.visible = true
        }
    }
    enabled: _ready
}
