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


    property bool _ready: status.synced
    property var _axisNames: ["x", "y", "z", "a", "b", "c", "u", "v", "w"]

    id: pathView

    function setViewMode(mode) {
        switch (mode) {
        case 0:
            camera.heading = -90
            camera.pitch = 0.00
            camera.projectionType = Camera3D.Orthographic
            break;
        }
    }

    camera: Camera3D {
        property real heading: 45
        property real pitch: 60
        property real distance: 200
        property vector3d lookCenter: Qt.vector3d((programExtents.maximum.x - programExtents.minimum.x)/2.0, (programExtents.maximum.y - programExtents.minimum.y)/2.0, 0)
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
        position.x: camera.lookCenter.x
        position.y: camera.lookCenter.y
        position.z: 2*boundingBox.maximum.z - boundingBox.minimum.z
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
        color: pathView.colors["grid"]
        minimum.x: boundingBox.minimum.x
        minimum.y: boundingBox.minimum.y
        minimum.z: 0.0
        maximum: boundingBox.maximum
        intervalX: 5
        intervalY: 5
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
        //position.x: boundingBox.minimum.x - 3
        //position.y: boundingBox.minimum.y - 3
        //position.z: boundingBox.minimum.z
        position: Qt.vector3d(0.0, 0.0, 0.0)
        textSize: 3
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
        model: (pathView.model !== undefined) ? pathView.model : tmpModel
    }

    GCodeProgramModel {
        id: tmpModel
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
