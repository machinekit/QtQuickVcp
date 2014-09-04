import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

Item {
    property alias model: listView.model
    property color selectedColor: "lightblue"
    property color activeColor: "lightcoral"
    property color executedColor: "lightgreen"
    property alias font: dummyLabel.font

    id: root

    SystemPalette { id: systemPalette }

    Rectangle {
        id: lineNumberBackground
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: Screen.pixelDensity * 11
        color: systemPalette.window
    }

    Rectangle {
        id: normalBackground
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: lineNumberBackground.right
        anchors.right: parent.right
        color: systemPalette.light
    }

    Label {
        id: dummyLabel
    }

    ScrollView {
        frameVisible: true
        anchors.fill: parent

        ListView {
            id: listView
            anchors.fill: parent
            delegate:
                Item {
                    property bool lineActive: active

                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: dummyLabel.height

                    /*onLineActiveChanged: {
                        if (lineActive) {
                            listView.positionViewAtIndex(index, ListView.Center)
                        }
                    }*/

                    Item {
                        id: lineNumberRect
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: lineNumberBackground.width

                        Label {
                            anchors.fill: parent
                            anchors.rightMargin: 5
                            text: lineNumber
                            horizontalAlignment: Text.AlignRight
                            color: selected ? "white" : label.color
                            font: dummyLabel.font
                        }
                    }

                    Rectangle {
                        color: selected ? root.selectedColor : (active ? root.activeColor : (executed ? root.executedColor : "transparent"))
                        anchors.left: lineNumberRect.right
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom


                        Label {
                            id: label
                            anchors.fill: parent
                            anchors.leftMargin: 5
                            text: gcode
                            font: dummyLabel.font
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: selected = !selected
                    }
            }
        }
    }
}
