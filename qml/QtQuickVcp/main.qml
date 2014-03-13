import QtQuick 2.1
import QtQuick.Controls 1.0
import Hal 1.0

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
        onClicked: serviceDiscovery.startDiscovery()
    }

    ServiceDiscovery {
        id: serviceDiscovery

        port: 10042
        retryTime: 500
        maxWait: 2000
        trace: true
        instance: 0

        services: [
            Service {
                id: halrcompService
                type: Service.ST_STP_HALRCOMP
                minVersion: 0
            },
            Service {
                id: rcommandService
                type: Service.ST_HAL_RCOMMAND
                minVersion: 0
            },
            Service {
                id: websocketService
                type: Service.ST_WEBSOCKET
                minVersion: 0
            }
        ]
    }

    Component {

    }
}
