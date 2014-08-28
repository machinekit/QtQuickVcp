import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

ProgressBar {
    property var file
    property string _mode: getMode()

    id: progressBar
    value: getProgress()

    function getMode() {
        if ((file !== undefined) && (file.transferState === ApplicationFile.UploadRunning)) {
            return "upload"
        }
        else {
            return ""
        }
    }

    function getText() {
        if (_mode == "upload") {
            return qsTr("Uploading file ") + file.fileName
        }
        else {
            return ""
        }
    }

    function getProgress() {
        if (_mode == "upload") {
            return applicationFile.progress
        }
        else {
            return 0.0
        }
    }

    Label {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: cancelButton.left
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideRight
        text: parent.getText()
    }

    ToolButton {
        id: cancelButton
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        iconSource: "qrc:Machinekit/Application/Controls/icons/dialog-cancel"
        visible: _mode == "upload"
        onClicked: {
            parent.file.abort()
        }
    }
}
