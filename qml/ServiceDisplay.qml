import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0

Rectangle {
    property string title: qsTr("Service Display")

    property list<Service> services: [
        Service {
            id: allServices
            type: ""
        }
    ]

    id: main

    color: systemPalette.window

    SystemPalette {
        id: systemPalette;
        colorGroup: enabled ? SystemPalette.Active : SystemPalette.Disabled
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Screen.logicalPixelDensity*3
        spacing: Screen.logicalPixelDensity*3

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: Screen.logicalPixelDensity*3

            model: allServices.items

            delegate: ColumnLayout {
                anchors.left: parent.left
                anchors.right: parent.right

                Label {
                    Layout.fillWidth: true
                    text: name
                    font.bold: true
                }
                Label {
                    Layout.fillWidth: true
                    text: txtRecords.join("\n")
                }
            }
        }
    }
}
