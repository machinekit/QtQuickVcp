import QtQuick 2.5
import Machinekit.Application 1.0
import QtQuick.Controls 2.2
import Machinekit.PathView 1.0
import "../src/pathview"

ConnectedTestBase {
    id: root

    PathView3D {
        anchors.fill: parent
        anchors.margins: 10

        visible: true
        enabled: true
        cameraZoom: 1

        LatheTool3D {
            id: latheTool
            orientation: 9
            diameter: 2.0
            frontangle: 0.0
            backangle: 70.0
        }
   }
}
