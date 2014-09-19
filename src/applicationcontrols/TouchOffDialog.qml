import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import Machinekit.Application 1.0

Dialog {
    property var status: {"synced": false}
    property var command: {"connected": false}
    property int axis: 0
    property var axisNames: ["X", "Y", "Z", "A", "B", "C", "U", "V", "W"]
    property var _axisNames: ["x", "y", "z", "a", "b", "c", "u", "v", "w"]

    property bool _ready: status.synced && command.connected
    property bool _done: true

    id: dialog
    title: qsTr("Touch Off")
    standardButtons: StandardButton.Ok | StandardButton.Cancel
    modality: Qt.ApplicationModal

    onVisibleChanged: {
        if (visible) {
            _done = false
            coordinateSpin.value = 0
            coordinateSystemCombo.currentIndex = 0
        }
    }

    onAccepted: {
        if (_ready && !_done) {
            if (status.task.taskMode !== ApplicationStatus.TaskModeMdi) {
                command.setTaskMode('execute', ApplicationCommand.TaskModeMdi)
            }
            var axisName = _axisNames[axis]
            var position = status.motion.position[axisName] - status.motion.g92Offset[axisName] - status.io.toolOffset[axisName]
            var newOffset = (position - coordinateSpin.value) / status.config.axis[axis].units
            var mdi = "G10 L2 P" + (coordinateSystemCombo.currentIndex + 1) + " " + axisNames[axis] + newOffset.toFixed(6)
            command.executeMdi('execute', mdi)
        }

        _done = true
    }

    ColumnLayout {
        anchors.fill: parent
        Label {
            text: qsTr("Enter ") + dialog.axisNames[dialog.axis] + qsTr(" coordinate relative to workpiece:")
        }
        SpinBox {
            id: coordinateSpin
            decimals: 4
            minimumValue: -9999999
            maximumValue: 9999999
        }
        RowLayout {
            Label {
                text: qsTr("Coordinate system:")
            }
            ComboBox {
                id: coordinateSystemCombo
                model: ListModel {
                    ListElement { text: "P1 G54" }
                    ListElement { text: "P2 G55" }
                    ListElement { text: "P3 G56" }
                    ListElement { text: "P4 G57" }
                    ListElement { text: "P5 G58" }
                    ListElement { text: "P6 G59" }
                    ListElement { text: "P7 G59.1" }
                    ListElement { text: "P8 G59.2" }
                    ListElement { text: "P9 G59.3" }
                }
            }
        }
    }
}
