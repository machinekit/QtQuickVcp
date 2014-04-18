import QtQuick 2.0
import Machinekit.HalRemote 1.0 as HAL
import Machinekit.HalRemote.Controls 1.0 as H

Item {
    property string title: "HAL Application Template"
    property string name: "halapp"
    property alias cmdUri: remoteComponent.cmdUri
    property alias updateUri: remoteComponent.updateUri
    property alias heartbeatPeriod: remoteComponent.heartbeatPeriod
    property bool autoDiscover: true
    property alias autoDiscoverTimeout: serviceDiscovery.maxWait
    property alias autoDiscoverRepeat: serviceDiscovery.repeat
    property alias autoDiscoverRetryTime: serviceDiscovery.retryTime
    property alias autoDiscoverPort: serviceDiscovery.port
    property bool ready: true
    property Item viewPage: defaultViewPage

    id: main

    width: 500
    height: 800


    Item {
        id: pageStack

        anchors.fill: parent

        H.DiscoveryPage {
            id: discoveryPage

            anchors.fill: parent
            remoteComponent: remoteComponent
            serviceDiscovery: serviceDiscovery
            halrcompService: halrcompService
            rcommandService: rcommandService
        }

        Item {
            id: defaultViewPage

            anchors.fill: parent
        }

        state: remoteComponent.ready?"connected":"discovery"

        states: [
            State {
                name: "discovery"
                PropertyChanges { target: discoveryPage; opacity: 1.0 }
                PropertyChanges { target: viewPage; opacity: 0.0 }
            },
            State {
                name: "connected"
                PropertyChanges { target: discoveryPage; opacity: 0.0 }
                PropertyChanges { target: viewPage; opacity: 1.0 }
            }
        ]

        transitions: Transition {
                PropertyAnimation { duration: 500; properties: "opacity"; easing.type: Easing.InCubic }
            }
    }

    HAL.RemoteComponent {
        id: remoteComponent

        name: main.name
        cmdUri: rcommandService.uri
        updateUri: halrcompService.uri
        heartbeatPeriod: 3000
        ready: main.autoDiscover?(halrcompService.found && rcommandService.found):main.ready
        containerItem: viewPage
    }

    HAL.ServiceDiscovery {
        id: serviceDiscovery

        port: 10042
        retryTime: 500
        maxWait: 2000
        trace: false
        instance: 0
        running: main.autoDiscover
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
            }
        ]
    }
}
