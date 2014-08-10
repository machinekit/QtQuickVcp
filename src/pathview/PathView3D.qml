import QtQuick 2.0
import QtQuick.Window 2.0
import Machinekit.PathView 1.0

GLView3D {
    property var colors: {
        "tool_diffuse": Qt.rgba(0.6, 0.6, 0.6, 1.0),
        "tool_ambient": Qt.rgba(0.4, 0.4, 0.4, 0.5),
        "background": Qt.rgba(0.0, 0.0, 0.0, 1.0),
        "grid": Qt.rgba(0.15, 0.15, 0.15, 1.0),
        "small_origin": Qt.rgba(0.0, 1.0, 1.0, 0.5),
        "cone": Qt.rgba(1.0, 1.0, 1.0, 1.0),
        "cone_xy": Qt.rgba(0.0, 1.0, 0.0, 1.0),
        "cone_uv": Qt.rgba(0.0, 0.0, 1.0, 1.0),
        "axis_x": Qt.rgba(0.2, 1.0, 0.2, 1.0),
        "axis_y": Qt.rgba(1.0, 1.0, 0.2, 1.0),
        "axis_z": Qt.rgba(0.2, 0.2, 1.0, 1.0),
        "label_ok": Qt.rgba(1.0, 0.51, 0.53, 1.0),
        "label_limit": Qt.rgba(1.0, 0.21, 0.23, 1.0),
        "selected": Qt.rgba(0.0, 1.0, 1.0, 1.0),
        "lathetool": Qt.rgba(0.8, 0.8, 0.8, 1.0),
        "m1xx": Qt.rgba(0.5, 0.5, 1.0, 1.0),
        "dwell": Qt.rgba(1.0, 0.5, 0.5, 1.0),
        "overlay_foreground": Qt.rgba(1.0, 1.0, 1.0, 0.75),
        "overlay_background": Qt.rgba(0.0, 0.0, 0.0, 0.75),
        "straight_feed": Qt.rgba(1.0, 1.0, 1.0, 0.33),
        "straight_feed_xy": Qt.rgba(0.2, 1.0, 0.2, 0.33),
        "straight_feed_uv": Qt.rgba(0.2, 0.2, 1.0, 0.33),
        "arc_feed": Qt.rgba(1.0, 1.0, 1.0, 0.5),
        "arc_feed_xy": Qt.rgba(0.2, 1.0, 0.2, 0.33),
        "arc_feed_uv": Qt.rgba(0.2, 0.2, 1.0, 0.33),
        "traverse": Qt.rgba(0.3, 0.5, 0.5, 0.33),
        "traverse_xy": Qt.rgba(0.3, 0.5, 0.5, 0.33),
        "traverse_uv": Qt.rgba(0.3, 0.5, 0.5, 0.33),
        "backplotjog": Qt.rgba(1.0, 1.0, 0.0, 0.75),
        "backplotprobing": Qt.rgba(0.63, 0.13, 0.94, 0.75),
        "backplottraverse": Qt.rgba(0.3, 0.5, 0.5, 0.25),
        "backplottoolchange": Qt.rgba(1.0, 0.65, 0.0, 0.25),
        "backplotarc": Qt.rgba(0.75, 0.25, 0.5, 0.75),
        "backplotfeed": Qt.rgba(0.75, 0.25, 0.25, 0.75)
    }

    property alias model: path.model

    id: pathView

    camera: Camera3D {
        property real heading: 45
        property real pitch: 60
        property real distance: 50
        property vector3d lookCenter: Qt.vector3d(boundingBox.size.x/2.0, boundingBox.size.y/2.0, 0)
        id: camera
        projectionType: Camera3D.Perspective
        eye: Qt.vector3d(distance * Math.sin(Math.PI*pitch/180) * Math.cos(Math.PI*heading/180) + lookCenter.x,
                         distance * Math.sin(Math.PI*pitch/180) * Math.sin(Math.PI*heading/180) + lookCenter.y,
                         distance * Math.cos(Math.PI*pitch/180))
        center: lookCenter
        upVector: Qt.vector3d(0, 0, 1)
    }

    light: Light3D {
        id: light
        position: Qt.vector3d(2.0, 2.0, 5.0)
        intensities: Qt.vector3d(1.0, 1.0, 1.0)
        ambientCoefficient: 0.5
        attenuation: 0.01
    }

    Sphere3D {
        id: smallOrigin
        radius: 0.05
        position: Qt.vector3d(2.0, 2.4, 0.2)
        color: pathView.colors["small_origin"]
    }

    Cylinder3D {
        id: tool
        position: Qt.vector3d(0.0, 0.0, 2.0)
        cone: true
        radius: 0.4
        color: pathView.colors["tool_diffuse"]
        rotationAngle: 180
        rotationAxis: Qt.vector3d(1,0,0)

        Timer {
            property real count: 0
            running: true
            repeat: true
            interval: 33
            onTriggered: {
                count += 0.1
                parent.position.x = Math.cos(count)*2 + 2
                parent.position.y = Math.sin(count)*2 +2
            }
        }
    }

    Grid3D {
        color: pathView.colors["grid"]
        size: Qt.vector2d(boundingBox.size.x, boundingBox.size.y)
    }

    BoundingBox3D {
        id: boundingBox
        size: Qt.vector3d(10.0, 10.0, 10.0)
    }

    ProgramExtents3D {
        maximum: path.maximumExtents
        minimum: path.minimumExtents
        textSize: 0.3
        decimals: 2
        color: pathView.colors["label_ok"]
    }

    Coordinate3D {
        position: Qt.vector3d(-0.1, -0.1, 0.0)
        textSize: 0.5
        xAxisColor: pathView.colors["axis_x"]
        yAxisColor: pathView.colors["axis_y"]
        zAxisColor: pathView.colors["axis_z"]
    }

    Path3D {
        id: path
        arcFeedColor: pathView.colors["arc_feed"]
        straightFeedColor: pathView.colors["straight_feed"]
        traverseColor: pathView.colors["traverse"]
        selectedColor: pathView.colors["selected"]
    }

    PinchArea {
        property real startDistance: 0
        anchors.fill: parent

        onPinchStarted: startDistance = camera.distance
        onPinchUpdated: camera.distance = startDistance * pinch.scale

        MouseArea {
            anchors.fill: parent
            onWheel: camera.distance -= wheel.angleDelta.y/60

            property int lastY: 0
            property int lastX: 0

            onMouseXChanged: move()
            onMouseYChanged: move()

            onPressed: {
                lastY = mouseY
                lastX = mouseX
            }

            onClicked: pathView.readPixel(mouseX, mouseY)

            function move() {
                if (!pressed)
                    return

                var xOffset = lastX - mouseX
                var yOffset = lastY - mouseY

                camera.heading += xOffset*2 / Screen.pixelDensity
                camera.pitch += yOffset*2 / Screen.pixelDensity

                lastY = mouseY
                lastX = mouseX
            }
        }
    }
}
