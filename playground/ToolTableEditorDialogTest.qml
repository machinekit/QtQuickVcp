import QtQuick 2.0
import QtQuick.Controls 1.4

import "../src/applicationcontrols"

UnscaledTestBase {
    id: root

    ToolTableEditorDialog {
        id: dialog
        width: root.width
        height: root.height
        status: {"synced": true}
        command: {"connected": true}
    }

    Button {
        anchors.centerIn: parent
        text: "show"
        onClicked: dialog.visible = true
    }
}
