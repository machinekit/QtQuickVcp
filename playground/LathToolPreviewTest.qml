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
        cameraZoom: 8

        Sphere3D {
            radius: 3.0
            position: Qt.vector3d(15.0, 15.0, 2.0)
            visible: true
            color: "red"
        }

        LatheToolItem {
            id: latheTool
            orientation: 6
            diameter: 2.0
            frontangle: 0.0
            backangle: 45.0
        }
   }
}
