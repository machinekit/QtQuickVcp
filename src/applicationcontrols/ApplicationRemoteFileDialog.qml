import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0
import Machinekit.Application 1.0
import Machinekit.Application.Controls 1.0

Dialog {
    property var core: null
    property var status: core === null ? {"synced": false} : core.status
    property var command: core === null ? {"ready": false} : core.command
    property var file: core === null ? {"localPath":"", "remotePath":"", "localFilePath":"", "ready":false} : core.file
    property var fileDialog: null

    property bool _ready: status.synced && file.ready && (file.transferState === ApplicationFile.NoTransfer)

    function _openFile(row) {
        if (row < 0)
            return

        if (status.task.taskMode !== ApplicationStatus.TaskModeAuto)
            command.setTaskMode('execute', ApplicationCommand.TaskModeAuto)
        if (status.task.file !== "") {
            command.resetProgram('execute')
        }
        var fileName = tableView.model.getName(row)
        var newPath = file.remotePath + '/' + fileName
        command.openProgram('execute', newPath)
        dialog.close()
    }

    function _removeFile(row) {
        if (row < 0)
            return

        var fileName = tableView.model.getName(row)
        file.removeFile(fileName)
    }

    function _uploadFileDialog() {
        dialog.close()
        fileDialog.open()
    }

    function _uploadFile(url) {
        file.localFilePath = url
        file.startUpload()
    }

    SystemPalette { id: systemPalette }

    id: dialog
    title: qsTr("Remote files")

    contentItem: Rectangle {
        color: systemPalette.window

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Screen.pixelDensity

        TableView {
            id: tableView
            Layout.fillHeight: true
            Layout.fillWidth: true
            enabled: _ready
            model: file.model

            TableViewColumn {
                role: "name"
                title: qsTr("Name")
                width: dialog.width * 0.4
            }
            TableViewColumn {
                role: "size"
                title: qsTr("Size")
                width: dialog.width * 0.2
            }
            TableViewColumn {
                role: "lastModified"
                title: qsTr("Last Modified")
                width: dialog.width * 0.3
            }

            onDoubleClicked: _openFile(row)
            onPressAndHold: fileMenu.popup()
            Keys.onReturnPressed: _openFile(currentRow)
            Keys.onDeletePressed: _removeFile(currentRow)

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton
                onClicked: {
                    fileMenu.popup()
                }
            }

            DropArea {
                anchors.fill: parent
                onDropped: {
                    if (drop.hasUrls) {
                        _uploadFile(drop.urls[0])
                    }
                }
            }

            Menu {
                id: fileMenu
                MenuItem {
                    text: qsTr("Remove file")
                    enabled: _ready && (tableView.currentRow > -1)
                    onTriggered: _removeFile(tableView.currentRow)
                }
                MenuItem {
                    text: qsTr("Open file")
                    enabled: _ready && (tableView.currentRow > -1)
                    onTriggered: _openFile(tableView.currentRow)
                }
                MenuItem {
                    text: qsTr("Upload file...")
                    enabled: _ready
                    onTriggered: _uploadFileDialog()
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true

            ApplicationProgressBar {
                Layout.fillWidth: true
            }

            Item { Layout.fillWidth: true }

            Button {
                text: qsTr("Refresh")
                enabled: _ready
                iconName: "view-refresh"
                onClicked: file.refreshFiles()
            }

            Button {
                text: qsTr("Upload...")
                enabled: _ready
                iconName: "document-open"
                onClicked: _uploadFileDialog()
            }

            Button {
                text: qsTr("Remove")
                iconName: "edit-delete"
                enabled: _ready && (tableView.currentRow > -1)
                onClicked: _removeFile(tableView.currentRow)
            }

            Button {
                text: qsTr("Open")
                iconName: "document-open-remote"
                enabled: _ready && (tableView.currentRow > -1)
                onClicked: _openFile(tableView.currentRow)
            }

            Button {
                text: qsTr("Close")
                iconName: "dialog-close"
                onClicked: dialog.close()
            }
        }
    }
    }

    onVisibleChanged: {
        if (visible)
            file.refreshFiles()
    }

    Component.onCompleted: {
        if (core == null)
        {
            try {
                var x = applicationCore
                core = Qt.binding(function() {return x})
            }
            catch (err) {
            }
        }
    }
}

