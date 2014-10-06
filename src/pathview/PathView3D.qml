/****************************************************************************
**
** Copyright (C) 2014 Alexander Rössler
** License: LGPL version 2.1
**
** This file is part of QtQuickVcp.
**
** All rights reserved. This program and the accompanying materials
** are made available under the terms of the GNU Lesser General Public License
** (LGPL) version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** Contributors:
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/

import QtQuick 2.0
import QtQuick.Window 2.0
import Machinekit.Application 1.0
import Machinekit.PathView 1.0

GLView3D {
    property alias status: object.status
    property alias model: pathViewObject.gcodeProgramModel
    property var colors: {
        "tool_diffuse": Qt.rgba(0.6, 0.6, 0.6, 1.0),
        "tool_ambient": Qt.rgba(0.4, 0.4, 0.4, 0.5),
        "background": Qt.rgba(0.0, 0.0, 0.0, 1.0),
        "grid": Qt.rgba(0.15, 0.15, 0.15, 1.0),
        "grid_min": Qt.rgba(0.05, 0.05, 0.05, 1.0),
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
    property int axes: _ready ? status.config.axes : 3
    property string viewMode: "Perspective"
    property real zoom: 0.95

    property bool _ready: status.synced
    property var _axisNames: ["x", "y", "z", "a", "b", "c", "u", "v", "w"]

    id: pathView

    camera: Camera3D {
        property real heading: -135
        property real pitch: 60
        property real distance: boundingBox.size.length() * 4.5

        id: camera
        projectionType: {
            switch (pathView.viewMode) {
            case "Top":
            case "RotatedTop":
            case "Front":
            case "Side":
                return Camera3D.Orthographic
            case "Perspective":
                return Camera3D.Perspective
            default:
                return Camera3D.Perspective
            }
        }
        eye: {
            switch (pathView.viewMode) {
            case "Top":
            case "RotatedTop":
                return Qt.vector3d(boundingBox.center.x, boundingBox.center.y, 1000)
            case "Front":
                return Qt.vector3d(boundingBox.center.x, -1000, boundingBox.center.z)
            case "Side":
                return Qt.vector3d(1000, boundingBox.center.y, boundingBox.center.z)
            case "Perspective":
                return Qt.vector3d(distance * Math.sin(Math.PI*pitch/180) * Math.cos(Math.PI*heading/180) / pathView.zoom + boundingBox.center.x,
                                   distance * Math.sin(Math.PI*pitch/180) * Math.sin(Math.PI*heading/180) / pathView.zoom + boundingBox.center.y,
                                   distance * Math.cos(Math.PI*pitch/180) / pathView.zoom + boundingBox.center.z)
            default:
                return  Qt.vector3d(0,0,0)
            }
        }
        center: {
            switch (pathView.viewMode) {
            case "Top":
            case "RotatedTop":
            case "Perspective":
                return boundingBox.center //Qt.vector3d(boundingBox.center.x, boundingBox.center.y, 0)
            case "Front":
                return Qt.vector3d(boundingBox.center.x, 0, boundingBox.center.z)
            case "Side":
                return Qt.vector3d(0, boundingBox.center.y, boundingBox.center.z)
            default:
                return Qt.vector3d(0,0,0)
            }
        }
        upVector: {
            switch (pathView.viewMode) {
            case "Top":
                return Qt.vector3d(0, 1, 0)
            case "RotatedTop":
                return Qt.vector3d(-1, 0, 0)
            default: Qt.vector3d(0, 0, 1)
            }
        }
        viewSize: {
            var side
            switch (pathView.viewMode) {
            case "Top":
            case "RotatedTop":
                side = Math.max(boundingBox.size.x, boundingBox.size.y) / pathView.zoom
                return Qt.size(side, side)
            case "Front":
                side = Math.max(boundingBox.size.x, boundingBox.size.z) / pathView.zoom
                return Qt.size(side, side)
            case "Side":
                side = Math.max(boundingBox.size.y, boundingBox.size.z) / pathView.zoom
                return Qt.size(side, side)
            default:
                return Qt.size(1,1)
            }
        }
        nearPlane: 5.0
        farPlane: 100000.0
    }

    light: Light3D {
        id: light
        position: boundingBox.center
        intensities: Qt.vector3d(1.0, 1.0, 1.0)
        ambientCoefficient: 0.7
        attenuation: 0.01
    }

    ApplicationObject {
        id: object
    }

    PathViewObject {
        id: pathViewObject
    }

    Sphere3D {
        id: smallOrigin
        radius: 0.05
        position: Qt.vector3d(2.0, 2.4, 0.2)
        color: pathView.colors["small_origin"]
    }

    Cylinder3D {
        id: tool
        position.x: _ready ? status.motion.position.x : 0
        position.y: _ready ? status.motion.position.y : 0
        position.z: (_ready ? status.motion.position.z : 0) + height
        cone: true
        radius: 3
        height: 8
        color: pathView.colors["tool_diffuse"]
        rotationAngle: 180
        rotationAxis: Qt.vector3d(1,0,0)
    }

    Grid3D {
        colorAxis1: pathView.colors["grid"]
        colorAxis1Min: pathView.colors["grid_min"]
        colorAxis2: pathView.colors["grid"]
        colorAxis2Min: pathView.colors["grid_min"]
        minimum: (plane == "XZ") ? Qt.vector3d(boundingBox.minimum.x,
                                               boundingBox.maximum.y,
                                               boundingBox.minimum.z) :
                                   boundingBox.minimum
        maximum: boundingBox.maximum
        intervalAxis1: 5
        intervalAxis1Min: intervalAxis1 / 5
        intervalAxis2: 5
        intervalAxis2Min: intervalAxis2 / 5
        plane: {
            switch (pathView.viewMode) {
            case "Top":
            case "RotatedTop":
                return "XY"
            case "Front":
                return "XZ"
            case "Side":
                return "YZ"
            default:
                return "XZ"
            }
        }
        visible: pathView.viewMode != "Perspective"

        onPaint: coordinates.requestPaint()
    }

    BoundingBox3D {
        id: boundingBox
        axes: pathView.axes
        minimum.x: _ready ? status.config.axis[0].minPositionLimit : 0
        minimum.y: (_ready && status.config.axis.length > 1) ? status.config.axis[1].minPositionLimit : 0
        minimum.z: (_ready && status.config.axis.length > 2) ? status.config.axis[2].minPositionLimit : 0
        maximum.x: _ready ? status.config.axis[0].maxPositionLimit : 0
        maximum.y: (_ready && status.config.axis.length > 1) ? status.config.axis[1].maxPositionLimit : 0
        maximum.z: (_ready && status.config.axis.length > 2) ? status.config.axis[2].maxPositionLimit : 0
        visible: pathView.viewMode == "Perspective"
    }

    ProgramExtents3D {
        id: programExtents
        axes: pathView.axes
        maximum: path.maximumExtents
        minimum: path.minimumExtents
        textSize: 5
        decimals: 2
        color: pathView.colors["label_ok"]
        limitColor: pathView.colors["label_limit"]
    }

    Coordinate3D {
        id: coordinates
        axes: pathView.axes
        position: Qt.vector3d(0.0, 0.0, 0.0)
        textSize: 3
        xAxisColor: pathView.colors["axis_x"]
        yAxisColor: pathView.colors["axis_y"]
        zAxisColor: pathView.colors["axis_z"]
        xAxisRotation: (pathView.viewMode == "Front") ? 90 : 0
        yAxisRotation: (pathView.viewMode == "Side") ? 90 : 0
        zAxisRotation: (pathView.viewMode == "Side") ? 90 : 0
    }

    Path3D {
        id: path
        arcFeedColor: pathView.colors["arc_feed"]
        straightFeedColor: pathView.colors["straight_feed"]
        traverseColor: pathView.colors["traverse"]
        selectedColor: pathView.colors["selected"]
        model: (pathView.model !== undefined) ? pathView.model : tmpModel
    }

    GCodeProgramModel {
        id: tmpModel
    }

    PinchArea {
        property real startZoom: 0
        anchors.fill: parent

        onPinchStarted: startZoom = pathView.zoom
        onPinchUpdated: pathView.zoom = startZoom * pinch.scale

        MouseArea {
            anchors.fill: parent
            onWheel: pathView.zoom *= (1 + wheel.angleDelta.y/1200)

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
