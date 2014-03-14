import QtQuick 2.1
import QtQuick.Controls 1.1
import Hal 1.0 as HAL

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480

    Item {
        id: pageStack

        anchors.fill: parent

        StartPage {
            id: startPage

            anchors.fill: parent
        }

        ViewPage {
            id: viewPage

            anchors.fill: parent
        }

        state: testComponent.ready?"connected":"discovery"

        states: [
            State {
                name: "discovery"
                PropertyChanges { target: startPage; opacity: 1.0 }
                PropertyChanges { target: viewPage; opacity: 0.0 }
            },
            State {
                name: "connected"
                PropertyChanges { target: startPage; opacity: 0.0 }
                PropertyChanges { target: viewPage; opacity: 1.0 }
            }
        ]

        transitions: Transition {
                PropertyAnimation { duration: 500; properties: "opacity"; easing.type: Easing.InCubic }
            }
    }

    HAL.RemoteComponent {
        id: testComponent

        name: "test"
        cmdUri: rcommandService.uri
        updateUri: halrcompService.uri
        heartbeatPeriod: 3000
        ready: halrcompService.found && rcommandService.found
        containerItem: viewPage
    }

    HAL.ServiceDiscovery {
        id: serviceDiscovery

        port: 10042
        retryTime: 500
        maxWait: 2000
        trace: true
        instance: 0
        running: true
        repeat: false

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
}
