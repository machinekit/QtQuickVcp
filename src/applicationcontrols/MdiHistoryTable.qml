import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import Machinekit.Application 1.0
import Machinekit.Application.Controls 1.0

TableView {
    property alias core: object.core
    property alias history: object.mdiHistory

    signal commandSelected(string command)
    signal commandTriggered(string command)

    id: mdiTableView
    Layout.fillWidth: true
    Layout.fillHeight: true
    model: object.mdiHistory.model
    headerVisible: false

    onClicked: {
        commandSelected(model[row].command)
    }
    onDoubleClicked: {
        commandTriggered(model[row].command)
    }
    onRowCountChanged: {
        mdiTableView.positionViewAtRow(rowCount-1, ListView.End)
    }

    Keys.onReturnPressed: {
        if (currentRow > -1) {
            commandTriggered(model[currentRow].command)
        }
    }

    ApplicationObject {
        id: object
    }

    TableViewColumn{ role: "command"  ; title: ""  }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            clearMenu.popup()
        }
    }

    Menu {
        id: clearMenu
        MenuItem { action: clearListAction }
        Action {
            id: clearListAction
            text: qsTr("Clear list")
            onTriggered: history.clear()
        }
    }
}
