import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import Machinekit.VideoView 1.0
import Machinekit.Service 1.0

Tab {
    id: main
    title: "Video"
    active: true

    Item {
        Service {
            id: videoService
            type: "video"
            required: false
        }

        ColumnLayout {
            anchors.margins: 10
            anchors.fill: parent
            spacing: 0

            MjpegStreamerClient {
                id: mjpegStreamerClient
                renderTarget: PaintedItem.FramebufferObject

                Layout.fillWidth: true
                Layout.fillHeight: true

                visible: videoService.ready
                videoUri: videoService.uri
                ready: visible && onOffSwitch.checked

                Label {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: Screen.logicalPixelDensity
                    text: parent.fps
                }

                Label {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.margins: Screen.logicalPixelDensity
                    text: parent.time.toTimeString()
                }

                Switch {
                    id: onOffSwitch
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.margins: Screen.logicalPixelDensity
                }
            }

            Label {
                id: infoLabel

                Layout.fillHeight: true
                Layout.fillWidth: true
                visible: !videoService.ready
                text: qsTr("Webcam not available")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}

