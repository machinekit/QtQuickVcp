import QtQuick 2.0
import QtQuick.Controls 1.1

Item {
    width: 100
    height: 62

    Text {
        id: sStateText

        anchors.verticalCenter: parent.verticalCenter

        text: qsTr("sState: ") + testComponent.sState
    }
    Text {
        id: cStateText

        anchors.bottom: sStateText.top

        text: qsTr("cState: ") + testComponent.cState
    }
    RadioButton {
        id: runningCheck

        anchors.bottom: cStateText.top

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
            if (serviceDiscovery.running === false)
            {
                serviceDiscovery.startDiscovery()
            }
        }
    }
}
