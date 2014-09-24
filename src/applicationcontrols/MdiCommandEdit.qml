import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

RowLayout {
    property alias text: mdiTextField.text
    property alias action: mdiCommandAction
    property alias core: mdiCommandAction.core
    property alias status: mdiCommandAction.status
    property alias command: mdiCommandAction.command
    property alias mdiHistory: mdiCommandAction.mdiHistory

    Layout.fillWidth: true
    Layout.fillHeight: false

    TextField {
        id: mdiTextField
        Layout.fillWidth: true
        Layout.fillHeight: true
        onAccepted: {
            mdiCommandAction.trigger()
        }
    }

    Button {
        Layout.fillHeight: true
        Layout.fillWidth: false
        action: mdiCommandAction
    }

    MdiCommandAction {
        id: mdiCommandAction
        mdiCommand: mdiTextField.text
    }
}
