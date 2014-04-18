import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import Machinekit.HalRemote 1.0 as HAL

Item {
    property variant remoteComponent
    property variant serviceDiscovery
    property variant halrcompService
    property variant rcommandService

    id: main

    width: 100
    height: 62

    ColumnLayout {
        anchors.margins: 10
        anchors.fill: parent
        spacing: 5

        Text {
            id: sStateText

            Layout.fillWidth: true
            text: qsTr("sState: ") + remoteComponent.sState
        }

        Text {
            id: cStateText

            Layout.fillWidth: true
            text: qsTr("cState: ") + remoteComponent.cState
        }

        RadioButton {
            id: runningCheck

            Layout.fillWidth: true
            text: qsTr("Discovery running")
            checked: serviceDiscovery.running
        }

        RadioButton {
            id: halrcompServiceCheck

            Layout.fillWidth: true
            text: qsTr("ST_STP_HALRCOMP command discovered")
            checked: halrcompService.found
        }

        RadioButton {
            id: rcommandServiceCheck

            Layout.fillWidth: true
            text: qsTr("ST_HAL_RCOMMAND command discovered")
            checked: rcommandService.found
        }

        Button {
            id: discoverButton

            Layout.fillWidth: true
            text: qsTr("Discover")
            onClicked: {
                if (serviceDiscovery.running === false)
                {
                    serviceDiscovery.startDiscovery()
                }
            }
        }
    }
}
