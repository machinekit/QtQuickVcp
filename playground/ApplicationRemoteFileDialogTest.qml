import QtQuick 2.0
import QtQuick.Controls 1.4

import "../src/applicationcontrols"

UnscaledTestBase {
    id: root

    ApplicationRemoteFileDialog {
        id: dialog
        width: root.width
        height: root.height
        file: {
            "model": listModel
        }
    }

    ListModel {
        id: listModel
        ListElement {
            name: "file.ngc"
            size: "10k"
            lastModified: "gestern"
            dir: false
        }

        ListElement {
            name: "folder"
            size: "2k"
            lastModified: "gestern"
            dir: true
        }

        function getIsDir(row) {
            return get(row).dir;
        }
    }

    Button {
        anchors.centerIn: parent
        text: "show"
        onClicked: dialog.visible = true
    }

    Component.onCompleted: dialog.visible = true
}
