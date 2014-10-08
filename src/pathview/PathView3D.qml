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
        "tool_diffuse": Qt.rgba(0.6, 0.6, 0.6, 0.8),
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
    property real sizeFactor: _ready ? status.config.linearUnits: 1

    property bool programVisible: true
    property bool gridVisible: true
    property bool rapidsVisible: true
    property bool alphaBlendProgram: true
    property bool livePlotVisible: true
    property bool toolVisible: true
    property bool programExtentsVisible: true
    property bool machineLimitsVisible: true
    property bool coordinateVisible: true
    property bool offsetsVisible: object.settings.initialized && object.settings.values.dro.showOffsets

    property bool _ready: status.synced
    property var _axisNames: ["x", "y", "z", "a", "b", "c", "u", "v", "w"]

    id: pathView

    camera: Camera3D {
        property real heading: -135
        property real pitch: 60
        property real distance: (programExtents.valid ? (programExtents.size.length() + 40 * sizeFactor) : boundingBox.size.length()) * 4.5

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
                if (programExtents.valid) {
                    return Qt.vector3d(programExtents.center.x + programExtents.position.x,
                                       programExtents.center.y + programExtents.position.y,
                                       1000)
                } else {
                    return Qt.vector3d(boundingBox.center.x, boundingBox.center.y, 1000)
                }
            case "Front":
                if (programExtents.valid) {
                    return Qt.vector3d(programExtents.center.x + programExtents.position.x,
                                       -1000,
                                       programExtents.center.z + programExtents.position.z)
                } else {
                    return Qt.vector3d(boundingBox.center.x, -1000, boundingBox.center.z)
                }
            case "Side":
                if (programExtents.valid) {
                    return Qt.vector3d(1000,
                                       programExtents.center.y + programExtents.position.y,
                                       programExtents.center.z + programExtents.position.z)
                } else {
                    return Qt.vector3d(1000, boundingBox.center.y, boundingBox.center.z)
                }
            case "Perspective":
                var center
                if (programExtents.valid) {
                    center = programExtents.center.plus(programExtents.position)
                } else {
                    center = boundingBox.center
                }
                return Qt.vector3d(distance * Math.sin(Math.PI*pitch/180) * Math.cos(Math.PI*heading/180) / pathView.zoom + center.x,
                                   distance * Math.sin(Math.PI*pitch/180) * Math.sin(Math.PI*heading/180) / pathView.zoom + center.y,
                                   distance * Math.cos(Math.PI*pitch/180) / pathView.zoom + center.z)
            default:
                return  Qt.vector3d(0,0,0)
            }
        }
        center: {
            var center
            switch (pathView.viewMode) {
            case "Top":
            case "RotatedTop":
            case "Perspective":
                if (programExtents.valid) {
                    return programExtents.center.plus(programExtents.position)
                } else {
                    return boundingBox.center
                }
            case "Front":
                if (programExtents.valid) {
                    center = programExtents.center.plus(programExtents.position)
                    return Qt.vector3d(center.x, 0, center.z)
                } else {
                    return Qt.vector3d(boundingBox.center.x, 0, boundingBox.center.z)
                }
            case "Side":
                if (programExtents.valid) {
                    center = programExtents.center.plus(programExtents.position)
                    return Qt.vector3d(0, center.y, center.z)
                } else {
                    return Qt.vector3d(0, boundingBox.center.y, boundingBox.center.z)
                }
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
            var u
            var v
            var viewRatio
            var objectRatio

            switch (pathView.viewMode) {
            case "Top":
            case "RotatedTop":
                if (programExtents.valid) {
                    side = (Math.max(programExtents.size.x, programExtents.size.y) + 40 * sizeFactor) / pathView.zoom
                } else {
                    side = Math.max(boundingBox.size.x, boundingBox.size.y) / pathView.zoom
                }
                return Qt.size(side, side)
            case "Front":
                if (programExtents.valid) {
                    side = (Math.max(programExtents.size.x, programExtents.size.z)  + 40 * sizeFactor) / pathView.zoom
                } else {
                    side = Math.max(boundingBox.size.x, boundingBox.size.z) / pathView.zoom
                }
                return Qt.size(side, side)
            case "Side":
                if (programExtents.valid) {
                    side = (Math.max(programExtents.size.y, programExtents.size.z)  + 40 * sizeFactor) / pathView.zoom
                } else {
                    side = Math.max(boundingBox.size.y, boundingBox.size.z) / pathView.zoom
                }
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

    Cylinder3D {
        id: tool
        visible: pathView.toolVisible
        position.x: _ready ? status.motion.position.x - status.io.toolOffset.x : 0
        position.y: _ready ? status.motion.position.y - status.io.toolOffset.y : 0
        position.z: (_ready ? status.motion.position.z - status.io.toolOffset.z : 0) + height

        cone: true
        radius: 5 * pathView.sizeFactor
        height: 15 * pathView.sizeFactor
        color: pathView.colors["tool_diffuse"]
        rotationAngle: 180
        rotationAxis: Qt.vector3d(1,0,0)
    }

    Grid3D {
        visible: pathView.gridVisible && (pathView.viewMode != "Perspective")
        colorAxis1: pathView.colors["grid"]
        colorAxis1Min: pathView.colors["grid_min"]
        colorAxis2: pathView.colors["grid"]
        colorAxis2Min: pathView.colors["grid_min"]
        minimum: (plane == "XZ") ? Qt.vector3d(boundingBox.minimum.x,
                                               boundingBox.maximum.y,
                                               boundingBox.minimum.z) :
                                   boundingBox.minimum
        maximum: boundingBox.maximum
        intervalAxis1: 10 * sizeFactor
        intervalAxis1Min: intervalAxis1 / 5
        intervalAxis2: 10 * sizeFactor
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

        onPaint: coordinates.requestPaint()
    }

    BoundingBox3D {
        id: boundingBox
        visible: pathView.machineLimitsVisible
        axes: pathView.axes
        lineStippleLength: (pathView.viewMode == "Perspective") ? 0.02 * camera.distance / pathView.zoom : 0.02
        minimum.x: _ready ? status.config.axis[0].minPositionLimit : 0
        minimum.y: (_ready && status.config.axis.length > 1) ? status.config.axis[1].minPositionLimit : 0
        minimum.z: (_ready && status.config.axis.length > 2) ? status.config.axis[2].minPositionLimit : 0
        maximum.x: _ready ? status.config.axis[0].maxPositionLimit : 0
        maximum.y: (_ready && status.config.axis.length > 1) ? status.config.axis[1].maxPositionLimit : 0
        maximum.z: (_ready && status.config.axis.length > 2) ? status.config.axis[2].maxPositionLimit : 0
    }

    ProgramExtents3D {
        id: programExtents
        visible: pathView.programVisible && pathView.programExtentsVisible
        position: coordinates.position
        axes: pathView.axes
        maximum: path.maximumExtents
        minimum: path.minimumExtents
        limitMinimum: boundingBox.minimum.minus(position)
        limitMaximum: boundingBox.maximum.minus(position)
        textSize: 8 * pathView.sizeFactor
        decimals: 2
        color: pathView.colors["label_ok"]
        limitColor: pathView.colors["label_limit"]
        viewMode: pathView.viewMode
    }

    Coordinate3D {
        property var g5xOffset: status.synced ? status.motion.g5xOffset : {"x":0.12345, "y":0.234,"z":123.12,"a":324.3}
        property var g92Offset: status.synced ? status.motion.g92Offset : {"x":0.12345, "y":0.234,"z":123.12,"a":324.3}
        property int positionOffset: status.synced ? status.config.positionOffset : ApplicationStatus.RelativePositionOffset

        id: coordinates
        visible: pathView.coordinateVisible
        axes: pathView.axes
        position: getPosition()
        textSize: 14 * pathView.sizeFactor
        axesLength: 25 * pathView.sizeFactor
        xAxisColor: pathView.colors["axis_x"]
        yAxisColor: pathView.colors["axis_y"]
        zAxisColor: pathView.colors["axis_z"]
        xAxisRotation: (pathView.viewMode == "Front") ? 90 : 0
        yAxisRotation: (pathView.viewMode == "Side") ? 90 : 0
        zAxisRotation: (pathView.viewMode == "Side") ? 90 : 0

        function getPosition() {
            var positionVector = Qt.vector3d(0.0, 0.0, 0.0)

            if (positionOffset == ApplicationStatus.RelativePositionOffset)
            {
                positionVector.x = g5xOffset.x + g92Offset.x
                if (axes > 1) {
                    positionVector.y = g5xOffset.y + g92Offset.y
                }
                if (axes > 2) {
                    positionVector.z = g5xOffset.z + g92Offset.z
                }
            }

            return positionVector
        }
    }

    ProgramOffsets3D {
        id: programOffsets
        textSize: 14 * sizeFactor
        color: pathView.colors["small_origin"]
        g5xIndex: status.synced ? status.motion.g5xIndex : 1
        g5xOffset: status.synced ? status.motion.g5xOffset : {"x":0.12345, "y":0.234,"z":123.12,"a":324.3}
        g92Offset: status.synced ? status.motion.g92Offset : {"x":0.12345, "y":0.234,"z":123.12,"a":324.3}
        visible: pathView.offsetsVisible && (status.config.positionOffset === ApplicationStatus.RelativePositionOffset)
    }

    Sphere3D {
        id: smallOrigin
        visible: pathView.offsetsVisible
        radius: 2 * sizeFactor
        position: Qt.vector3d(0.0, 0.0, 0.0)
        color: pathView.colors["small_origin"]
    }

    Path3D {
        id: path
        visible: pathView.programVisible
        position: coordinates.position
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

                switch (pathView.viewMode)
                {
                case "Perspective":
                    camera.heading += xOffset*2 / Screen.pixelDensity
                    camera.pitch += yOffset*2 / Screen.pixelDensity
                    break
                case "Front":
                    console.log("X " + xOffset)
                    console.log("Y " + yOffset)
                    break
                default:
                    break
                }
                lastY = mouseY
                lastX = mouseX
            }
        }
    }
}
