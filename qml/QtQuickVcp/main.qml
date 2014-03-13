import QtQuick 2.1
import QtQuick.Controls 1.0
import Hal 1.0 as HAL

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480

    /*menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }*/

    RadioButton {
        id: runningCheck

        anchors.verticalCenter: parent.verticalCenter

        text: qsTr("Discovery running")
        checked: serviceDiscovery.running
    }
    RadioButton {
        id: halrcompServiceCheck

        anchors.bottom: runningCheck.top

        text: qsTr("ST_STP_HALRCOMP command discovered")
        checked: halrcompService.found
    }
    RadioButton {
        id: rcommandServiceCheck

        anchors.bottom: halrcompServiceCheck.top

        text: qsTr("ST_HAL_RCOMMAND command discovered")
        checked: rcommandService.found
    }
    Button {
        id: discoverButton

        anchors.bottom: rcommandServiceCheck.top

        text: qsTr("Discover")
        onClicked: {
            if ((serviceDiscovery.running === false)
                    && (halrcompService.found)
                    && (rcommandService.found))
            {
                testComponent.ready()
            }
            else
            {
                serviceDiscovery.startDiscovery()
            }
        }
    }

    HAL.ServiceDiscovery {
        id: serviceDiscovery

        port: 10042
        retryTime: 500
        maxWait: 2000
        trace: true
        instance: 0

        services: [
            HAL.Service {
                id: halrcompService
                type: HAL.Service.ST_STP_HALRCOMP
                minVersion: 0
            },
            HAL.Service {
                id: rcommandService
                type: HAL.Service.ST_HAL_RCOMMAND
                minVersion: 0
            },
            HAL.Service {
                id: websocketService
                type: HAL.Service.ST_WEBSOCKET
                minVersion: 0
            }
        ]
    }

    HAL.RemoteComponent {
        id: testComponent

        anchors.fill: parent

        name: "motorctrl"
        cmdUri: rcommandService.uri
        updateUri: halrcompService.uri
        heartbeatPeriod: 3000

        Button {
            id: myButton

            anchors.bottom: parent.bottom
            anchors.left: parent.left
            text: qsTr("Button")

            HAL.Pin {
                id: myPin

                name: "scope_trigger"
                type: HAL.Pin.HAL_BIT
                direction: HAL.Pin.HAL_OUT
                value: myButton.checked
            }
        }

        RadioButton {
            id: myRadio

            anchors.bottom: parent.bottom
            anchors.right: parent.right
            text: qsTr("RadioButton")

            HAL.Pin {
                id: myRadioPin

                name: "led1"
                type: HAL.Pin.HAL_BIT
                direction: HAL.Pin.HAL_IN
            }

            checked: myRadioPin.value
        }
    }
}
