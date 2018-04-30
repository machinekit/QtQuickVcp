import QtQuick 2.0
import "../src/applicationcontrols"

ConnectedTestBase {
    id: root

    DigitalReadOut {
        anchors.centerIn: parent
        offsetsVisible: true
        velocityVisible: true
        distanceToGoVisible: true
    }


    TextInput {
        anchors.left: parent.left
        anchors.top: parent.top
        text: "lathe: " + applicationCore.status.config.lathe
    }

}
