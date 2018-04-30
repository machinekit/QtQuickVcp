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
    property alias helper: object.helper
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
        "axis_y": Qt.rgba(1.0, 0.2, 0.2, 1.0),
        "axis_z": Qt.rgba(0.2, 0.2, 1.0, 1.0),
        "label_ok": Qt.rgba(1.0, 0.51, 0.53, 1.0),
        "label_limit": Qt.rgba(1.0, 0.21, 0.23, 1.0),
        "selected": Qt.rgba(0.0, 1.0, 1.0, 1.0),
        "active": Qt.rgba(1.0, 0.0, 0.0, 1.0),
        "lathetool": Qt.rgba(0.8, 0.8, 0.8, 0.1),
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
        "backplotfeed": Qt.rgba(0.75, 0.25, 0.25, 0.75),
        "machine_limits": Qt.rgba(1.0, 0.0, 0.0, 1.0)
    }
    property int axes: __ready ? status.config.axes : 3
    property string viewMode: __ready && status.config.lathe ? "Lathe" : "Perspective"
    property real cameraZoom: 0.95
    property vector3d cameraOffset: Qt.vector3d(0, 0, 0)
    property real cameraHeading: -135
    property real cameraPitch: 60
    property real sizeFactor: camera.distance / cameraZoom * 0.0015

    property bool zoomToProgram: object.settings.initialized && object.settings.values.preview.zoomToProgram
    property bool programVisible: object.settings.initialized && object.settings.values.preview.showProgram
    property bool gridVisible: object.settings.initialized && object.settings.values.preview.showGrid
    property bool programRapidsVisible: object.settings.initialized && object.settings.values.preview.showProgramRapids
    property bool alphaBlendProgram: object.settings.initialized && object.settings.values.preview.alphaBlendProgram
    property bool livePlotVisible: object.settings.initialized && object.settings.values.preview.showLivePlot
    property bool toolVisible: object.settings.initialized && object.settings.values.preview.showTool
    property bool programExtentsVisible: object.settings.initialized && object.settings.values.preview.showProgramExtents
    property bool machineLimitsVisible: object.settings.initialized && object.settings.values.preview.showMachineLimits
    property bool coordinateVisible: object.settings.initialized && object.settings.values.preview.showCoordinate
    property bool offsetsVisible: object.settings.initialized && object.settings.values.dro.showOffsets
    property real gridInterval: object.settings.initialized ? object.settings.values.preview.gridInterval : 100.0

    readonly property bool __ready: status.synced
    readonly property double __distanceFactor: helper.ready ? helper.distanceFactor : 1.0

    id: pathView

    enabled: object.settings.initialized && object.settings.values.preview.enable
    visible: enabled

    camera: camera3d
    light: light3d

    ApplicationObject {
        id: object
    }

    PathViewObject {
        id: pathViewObject
    }

    Camera3D {
            readonly property real heading: pathView.cameraHeading
            readonly property real pitch: pathView.cameraPitch
            readonly property real distance: ((programExtents.valid && zoomToProgram) ? (programExtents.size.length() + 40) : boundingBox.size.length()) * 4.5
            readonly property vector3d centerOffset: pathView.cameraOffset

            id: camera3d
            projectionType: {
                switch (pathView.viewMode) {
                case "Top":
                case "RotatedTop":
                case "Front":
                case "Side":
                case "Lathe":
                    return Camera3D.Orthographic;
                case "Perspective":
                    return Camera3D.Perspective;
                default:
                    return Camera3D.Perspective;
                }
            }
            eye: {
                var x = camera3d.centerOffset.x;
                var y = camera3d.centerOffset.y;
                var z = camera3d.centerOffset.z;

                switch (pathView.viewMode) {
                case "Top":
                case "RotatedTop":
                    z = 10000;
                    if (programExtents.valid && zoomToProgram) {
                        x += programExtents.center.x + programExtents.position.x;
                        y += programExtents.center.y + programExtents.position.y;
                    } else {
                        x += boundingBox.center.x;
                        y += boundingBox.center.y;
                    }
                    break
                case "Front":
                case "Lathe":
                    y = -10000;
                    if (programExtents.valid && zoomToProgram) {
                        x += programExtents.center.x + programExtents.position.x;
                        z += programExtents.center.z + programExtents.position.z;
                    } else {
                        x += boundingBox.center.x;
                        z += boundingBox.center.z;
                    }
                    break
                case "Side":
                    x = 10000
                    if (programExtents.valid && zoomToProgram) {
                        y += programExtents.center.y + programExtents.position.y;
                        z += programExtents.center.z + programExtents.position.z;
                    } else {
                        y += boundingBox.center.y;
                        z += boundingBox.center.z;
                    }
                    break
                case "Perspective":
                    if (programExtents.valid && zoomToProgram) {
                        x += programExtents.center.x + programExtents.position.x;
                        y += programExtents.center.y + programExtents.position.y;
                        z += programExtents.center.z + programExtents.position.z;
                    } else {
                        x += boundingBox.center.x;
                        y += boundingBox.center.y;
                        z += boundingBox.center.z;
                    }
                    x += distance * Math.sin(Math.PI*pitch/180) * Math.cos(Math.PI*heading/180) / pathView.cameraZoom;
                    y += distance * Math.sin(Math.PI*pitch/180) * Math.sin(Math.PI*heading/180) / pathView.cameraZoom;
                    z += distance * Math.cos(Math.PI*pitch/180) / pathView.cameraZoom;
                    break;
                default:
                    break;
                }

                return Qt.vector3d(x, y, z);
            }
            center: {
                var x = camera3d.centerOffset.x;
                var y = camera3d.centerOffset.y;
                var z = camera3d.centerOffset.z;


                switch (pathView.viewMode) {
                case "Top":
                case "RotatedTop":
                case "Perspective":
                    if (programExtents.valid && zoomToProgram) {
                        x += programExtents.center.x +  programExtents.position.x;
                        y += programExtents.center.y +  programExtents.position.y;
                        z += programExtents.center.z +  programExtents.position.z;
                    } else {
                        x += boundingBox.center.x;
                        y += boundingBox.center.y;
                        z += boundingBox.center.z;
                    }
                    break;
                case "Front":
                case "Lathe":
                    y = 0
                    if (programExtents.valid && zoomToProgram) {
                        x += programExtents.center.x + programExtents.position.x;
                        z += programExtents.center.z + programExtents.position.z;
                    } else {
                        x += boundingBox.center.x;
                        z += boundingBox.center.z;
                    }
                    break;
                case "Side":
                    x = 0
                    if (programExtents.valid && zoomToProgram) {
                        y = programExtents.center.y + programExtents.position.y;
                        z = programExtents.center.z + programExtents.position.z;
                    } else {
                        y += boundingBox.center.y;
                        z += boundingBox.center.z;
                    }
                    break;
                default:
                    break;
                }

                return Qt.vector3d(x,y,z);
            }
            upVector: {
                switch (pathView.viewMode) {
                case "Top":
                    return Qt.vector3d(0, 1, 0);
                case "RotatedTop":
                    return Qt.vector3d(-1, 0, 0);
                case "Lathe":
                    return Qt.vector3d(-1, 0, 0)
                default:
                    return Qt.vector3d(0, 0, 1);
                }
            }
            viewSize: {
                var side;
                var u;
                var v;
                var viewRatio;
                var objectRatio;

                switch (pathView.viewMode) {
                case "Top":
                case "RotatedTop":
                    if (programExtents.valid && zoomToProgram) {
                        side = (Math.max(programExtents.size.x, programExtents.size.y) + 40 * sizeFactor) / pathView.cameraZoom;
                    } else {
                        side = Math.max(boundingBox.size.x, boundingBox.size.y) / pathView.cameraZoom;
                    }
                    return Qt.size(side, side);
                case "Front":
                case "Lathe":
                    if (programExtents.valid && zoomToProgram) {
                        side = (Math.max(programExtents.size.x, programExtents.size.z)  + 40 * sizeFactor) / pathView.cameraZoom;
                    } else {
                        side = Math.max(boundingBox.size.x, boundingBox.size.z) / pathView.cameraZoom;
                    }
                    return Qt.size(side, side);
                case "Side":
                    if (programExtents.valid && zoomToProgram) {
                        side = (Math.max(programExtents.size.y, programExtents.size.z)  + 40 * sizeFactor) / pathView.cameraZoom;
                    } else {
                        side = Math.max(boundingBox.size.y, boundingBox.size.z) / pathView.cameraZoom;
                    }
                    return Qt.size(side, side);
                default:
                    return Qt.size(1,1);
                }
            }
            nearPlane: 5.0
            farPlane: 100000.0
        }

    Light3D {
        id: light3d
        position: boundingBox.center
        intensities: Qt.vector3d(1.0, 1.0, 1.0)
        ambientCoefficient: 0.7

        attenuation: 0.01
    }

    Cylinder3D {
        readonly property var toolInfo: __ready ? getToolInfo() : {"diameter": 0.0, "length": 0.0, "valid": false, "orientation": 0}
        readonly property real toolDiameter: toolInfo.diameter
        readonly property real toolLength: toolInfo.length
        readonly property int toolOrientation: toolInfo.orientation
        readonly property bool latheMode: pathView.viewMode == "Lathe"
        readonly property bool latheToolMode: latheMode && (toolOrientation != 0)

        function getToolInfo()
        {
            var toolInSpindle = status.io.toolInSpindle;
            var toolTable = status.io.toolTable;
            var diameter = 0.0;
            var length = 0.0;
            var valid = false;
            var orientation = 0;
            var frontangle = 0;
            var backangle = 0;
            for (var i = 0; i < toolTable.length; i++)
            {
                var tool = toolTable[i]
                if (tool.id === toolInSpindle)
                {
                    diameter = tool.diameter;
                    length = tool.offset.z;
                    orientation = tool.orientation;
                    frontangle = tool.frontangle;
                    backangle = tool.backangle;
                    valid = true;
                    break;
                }
            }

            return {"diameter": diameter, "length": length, "valid": valid, "orientation": orientation,
                    "frontangle": frontangle, "backangle": backangle};
        }

        id: tool
        visible: pathView.toolVisible && !latheToolMode
        position.x: __ready ? status.motion.position.x - status.io.toolOffset.x : 0
        position.y: __ready ? status.motion.position.y - status.io.toolOffset.y : 0
        position.z: (__ready ? status.motion.position.z - status.io.toolOffset.z : 0)

        cone: toolInfo.valid ? false : true
        radius: toolInfo.valid ? toolDiameter / 2.0 : 5.0 * pathView.sizeFactor
        height: toolInfo.valid ? toolLength : 10.0 * pathView.sizeFactor
        color: pathView.colors["tool_diffuse"]
        rotationVector: latheMode ? Qt.vector3d(0, 90.0, 0) : Qt.vector3d(0, 0, 0)
    }

    LatheTool3D {
        id: latheTool
        visible: pathView.toolVisible && tool.latheToolMode
        position.x: __ready ? status.motion.position.x - status.io.toolOffset.x : 0
        position.y: __ready ? status.motion.position.y - status.io.toolOffset.y : 0
        position.z: (__ready ? status.motion.position.z - status.io.toolOffset.z : 0)
        orientation: tool.toolInfo.valid ? tool.toolInfo.orientation : 0
        diameter: tool.toolInfo.valid ? tool.toolInfo.diameter : 0.0
        frontangle: tool.toolInfo.valid ? tool.toolInfo.frontangle: 0.0
        backangle: tool.toolInfo.valid ? tool.toolInfo.backangle: 0.0
        crossColor: pathView.colors["lathetool"]
        shapeColor: pathView.colors["tool_diffuse"]
    }

    Grid3D {
        visible: pathView.gridVisible && (pathView.viewMode !== "Perspective")
        colorAxis1: pathView.colors["grid"]
        colorAxis1Min: pathView.colors["grid_min"]
        colorAxis2: pathView.colors["grid"]
        colorAxis2Min: pathView.colors["grid_min"]
        minimum: (plane === "XZ") ? Qt.vector3d(boundingBox.minimum.x,
                                               boundingBox.maximum.y,
                                               boundingBox.minimum.z) :
                                   boundingBox.minimum
        maximum: boundingBox.maximum
        intervalAxis1: pathView.gridInterval
        intervalAxis1Min: intervalAxis1 / 5
        intervalAxis2: pathView.gridInterval
        intervalAxis2Min: intervalAxis2 / 5
        plane: {
            switch (pathView.viewMode) {
            case "Top":
            case "RotatedTop":
                return "XY";
            case "Front":
            case "Lathe":
                return "XZ";
            case "Side":
                return "YZ";
            default:
                return "XZ";
            }
        }

        onPaint: coordinates.requestPaint()
    }

    BoundingBox3D {
        id: boundingBox
        visible: pathView.machineLimitsVisible
        axes: pathView.axes
        color: pathView.colors["machine_limits"]
        lineStippleLength: (pathView.viewMode === "Perspective") ? 0.02 * camera.distance / pathView.cameraZoom : 0.02
        minimum.x: __ready ? status.config.axis[0].minPositionLimit : 0
        minimum.y: (__ready && status.config.axis.length > 1) ? status.config.axis[1].minPositionLimit : 0
        minimum.z: (__ready && status.config.axis.length > 2) ? status.config.axis[2].minPositionLimit : 0
        maximum.x: __ready ? status.config.axis[0].maxPositionLimit : 0
        maximum.y: (__ready && status.config.axis.length > 1) ? status.config.axis[1].maxPositionLimit : 0
        maximum.z: (__ready && status.config.axis.length > 2) ? status.config.axis[2].maxPositionLimit : 0
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
        textSize: 4 * pathView.sizeFactor
        decimals: (__ready && (status.interp.programUnits === ApplicationStatus.CanonUnitsInch)) ? 3 : 2
        color: pathView.colors["label_ok"]
        limitColor: pathView.colors["label_limit"]
        viewMode: pathView.viewMode
        scaleFactor: pathView.__distanceFactor
    }

    Coordinate3D {
        readonly property var g5xOffset: status.synced ? status.motion.g5xOffset : {"x":0.12345, "y":0.234,"z":123.12,"a":324.3}
        readonly property var g92Offset: status.synced ? status.motion.g92Offset : {"x":0.12345, "y":0.234,"z":123.12,"a":324.3}
        readonly property int positionOffset: status.synced ? status.config.positionOffset : ApplicationStatus.RelativePositionOffset

        id: coordinates
        visible: pathView.coordinateVisible
        axes: pathView.axes
        position: getPosition()
        textSize: 7 * pathView.sizeFactor
        axesLength: 20 * pathView.sizeFactor
        xAxisColor: pathView.colors["axis_x"]
        yAxisColor: pathView.colors["axis_y"]
        zAxisColor: pathView.colors["axis_z"]
        xAxisRotation: (pathView.viewMode === "Front" || pathView.viewMode === "Lathe") ? 90 : 0
        yAxisRotation: (pathView.viewMode === "Side") ? 90 : 0
        zAxisRotation: (pathView.viewMode === "Side") ? 90 : 0

        function getPosition() {
            var positionVector = Qt.vector3d(0.0, 0.0, 0.0);

            if (positionOffset === ApplicationStatus.RelativePositionOffset) {
                positionVector.x = g5xOffset.x + g92Offset.x;
                if (axes > 1) {
                    positionVector.y = g5xOffset.y + g92Offset.y;
                }
                if (axes > 2) {
                    positionVector.z = g5xOffset.z + g92Offset.z;
                }
            }

            return positionVector;
        }
    }

    ProgramOffsets3D {
        id: programOffsets
        textSize: 6 * sizeFactor
        color: pathView.colors["small_origin"]
        g5xIndex: status.synced ? status.motion.g5xIndex : 1
        g5xOffset: status.synced ? status.motion.g5xOffset : {"x":0.12345, "y":0.234,"z":123.12,"a":324.3}
        g92Offset: status.synced ? status.motion.g92Offset : {"x":0.12345, "y":0.234,"z":123.12,"a":324.3}
        visible: pathView.offsetsVisible && (status.config.positionOffset === ApplicationStatus.RelativePositionOffset)
        viewMode: pathView.viewMode
    }

    Sphere3D {
        id: smallOrigin
        visible: pathView.offsetsVisible
        radius: 0.75 * sizeFactor
        position: Qt.vector3d(0.0, 0.0, 0.0)
        color: pathView.colors["small_origin"]
    }

    Sphere3D {
        id: zeroOrigin
        visible: !coordinateVisible
        radius: 0.75 * sizeFactor
        position: coordinates.position
        color: pathView.colors["small_origin"]
    }

    Sphere3D {
        id: g5xOrigin
        visible: pathView.offsetsVisible
        radius: 0.75 * sizeFactor
        position: Qt.vector3d(coordinates.g5xOffset.x, coordinates.g5xOffset.y, coordinates.g5xOffset.z)
        color: pathView.colors["small_origin"]
    }

    Path3D {
        id: path
        visible: pathView.programVisible
        position: coordinates.position
        arcFeedColor: pathView.colors["arc_feed"]
        straightFeedColor: pathView.colors["straight_feed"]
        traverseColor: pathView.colors["traverse"]
        backplotArcFeedColor: pathView.colors["backplotarc"]
        backplotStraightFeedColor: pathView.colors["backplotfeed"]
        backplotTraverseColor: pathView.colors["backplottraverse"]
        selectedColor: pathView.colors["selected"]
        activeColor: pathView.colors["active"]
        traverseLineStippleLength: (pathView.viewMode === "Perspective") ? 0.015 * camera3d.distance / pathView.cameraZoom : 0.015
        arcDivision: __ready ? status.config.arcdivision : 16
        model: (pathView.model !== undefined) ? pathView.model : tmpModel
    }

    GCodeProgramModel {
        id: tmpModel
    }

    PinchArea {
        property real startZoom: 0
        anchors.fill: parent

        onPinchStarted: startZoom = pathView.cameraZoom
        onPinchUpdated: pathView.cameraZoom = startZoom * pinch.scale

        MouseArea {
            anchors.fill: parent
            onWheel: pathView.cameraZoom *= (1 + wheel.angleDelta.y/1200)

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
                if (!pressed) {
                    return;
                }

                var scaleFactor = camera3d.viewSize.height/pathView.height;
                var xOffset = lastX - Math.floor(mouseX);
                var yOffset = lastY - Math.floor(mouseY);

                switch (pathView.viewMode)
                {
                case "Perspective":
                    pathView.cameraHeading += xOffset * 2 / Screen.pixelDensity;
                    pathView.cameraPitch += yOffset * 2 / Screen.pixelDensity;
                    break;
                case "Front":
                    pathView.cameraOffset.x += xOffset * scaleFactor;
                    pathView.cameraOffset.z -= yOffset * scaleFactor;
                    break;
                case "Lathe":
                    pathView.cameraOffset.x += yOffset * scaleFactor;
                    pathView.cameraOffset.z += xOffset * scaleFactor;
                    break;
                case "Side":
                    pathView.cameraOffset.y += xOffset * scaleFactor;
                    pathView.cameraOffset.z -= yOffset * scaleFactor;
                    break;
                case "Top":
                    pathView.cameraOffset.x += xOffset * scaleFactor;
                    pathView.cameraOffset.y -= yOffset * scaleFactor;
                    break;
                case "RotatedTop":
                    pathView.cameraOffset.y += xOffset * scaleFactor;
                    pathView.cameraOffset.x += yOffset * scaleFactor;
                    break;
                default:
                    break;
                }
                lastY = mouseY;
                lastX = mouseX;
            }
        }
    }
}
