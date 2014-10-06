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
import Machinekit.PathView 1.0

Canvas3D {
    property vector3d minimum: Qt.vector3d(0.0, 0.0, 0.0)
    property vector3d maximum: Qt.vector3d(10.0, 10.0, 10.0)
    property real lineWidthAxis1: 2.0
    property real lineWidthAxis1Min: 0.5
    property real lineWidthAxis2: 2.0
    property real lineWidthAxis2Min: 0.5
    property color colorAxis1: "#333"
    property color colorAxis2: colorAxis1
    property color colorAxis1Min: "#111"
    property color colorAxis2Min: colorAxis1Min
    property real intervalAxis1: 1.0
    property real intervalAxis1Min: 0.2
    property real intervalAxis2: 1.0
    property real intervalAxis2Min: 0.2
    property bool enableAxis1: true
    property bool enableAxis2: true
    property bool enableAxis1Min: true
    property bool enableAxis2Min: true
    property bool alignToOrigin: true
    property string plane: "XY"

    id: root
    onPaint: {
        var size = Qt.vector3d(maximum.x - minimum.x, maximum.y - minimum.y, maximum.z - minimum.z)

        var posX
        var posY
        var posZ

        var xVector = Qt.vector3d(0.0, 0.0, 0.0)
        var yVector = Qt.vector3d(0.0, 0.0, 0.0)
        var zVector = Qt.vector3d(0.0, 0.0, 0.0)

        var xEnabled = false
        var xMinEnabled = false
        var yEnabled = false
        var yMinEnabled = false
        var zEnabled = false
        var zMinEnabled = false

        var xInterval
        var xMinInterval
        var xColor
        var xMinColor
        var xLineWidth
        var xMinLineWidth
        var yInterval
        var yMinInterval
        var yColor
        var yMinColor
        var yLineWidth
        var yMinLineWidth
        var zInterval
        var zMinInterval
        var zColor
        var zMinColor
        var zLineWidth
        var zMinLineWidth

        if (plane == "XY")
        {
            xEnabled = root.enableAxis1
            xMinEnabled = root.enableAxis1Min
            xInterval = root.intervalAxis1
            xMinInterval = root.intervalAxis1Min
            xColor = root.colorAxis1
            xMinColor = root.colorAxis1Min
            xLineWidth = root.lineWidthAxis1
            xMinLineWidth = root.lineWidthAxis1Min

            yEnabled = root.enableAxis2
            yMinEnabled = root.enableAxis2Min
            yInterval = root.intervalAxis2
            yMinInterval = root.intervalAxis2Min
            yColor = root.colorAxis2
            yMinColor = root.colorAxis2Min
            yLineWidth = root.lineWidthAxis2
            yMinLineWidth = root.lineWidthAxis2Min

            xVector.y = size.y
            yVector.x = size.x
        }

        if (plane == "XZ")
        {
            xEnabled = root.enableAxis1
            xMinEnabled = root.enableAxis1Min
            xInterval = root.intervalAxis1
            xMinInterval = root.intervalAxis1Min
            xColor = root.colorAxis1
            xMinColor = root.colorAxis1Min
            xLineWidth = root.lineWidthAxis1
            xMinLineWidth = root.lineWidthAxis1Min

            zEnabled = root.enableAxis2
            zMinEnabled = root.enableAxis2Min
            zInterval = root.intervalAxis2
            zMinInterval = root.intervalAxis2Min
            zColor = root.colorAxis2
            zMinColor = root.colorAxis2Min
            zLineWidth = root.lineWidthAxis2
            zMinLineWidth = root.lineWidthAxis2Min

            xVector.z = size.z
            zVector.x = size.x
        }

        if (plane == "YZ")
        {
            yEnabled = root.enableAxis1
            yMinEnabled = root.enableAxis1Min
            yInterval = root.intervalAxis1
            yMinInterval = root.intervalAxis1Min
            yColor = root.colorAxis1
            yMinColor = root.colorAxis1Min
            yLineWidth = root.lineWidthAxis1
            yMinLineWidth = root.lineWidthAxis1Min

            zEnabled = root.enableAxis2
            zMinEnabled = root.enableAxis2Min
            zInterval = root.intervalAxis2
            zMinInterval = root.intervalAxis2Min
            zColor = root.colorAxis2
            zMinColor = root.colorAxis2Min
            zLineWidth = root.lineWidthAxis2
            zMinLineWidth = root.lineWidthAxis2Min

            yVector.z = size.z
            zVector.y = size.y
        }

        context.prepare(this)
        context.reset()
        context.translate(minimum)
        context.beginUnion()

        if (xMinEnabled) {
            posX = alignToOrigin ? (Math.abs(minimum.x) % xMinInterval) : 0
            context.color(xMinColor)
            context.lineWidth(xMinLineWidth)
            context.beginUnion()
            while (posX <= size.x) {
                context.lineFromTo(posX, 0.0, 0.0,
                                   posX, xVector.y, xVector.z)
                posX += xMinInterval
            }
            context.endUnion()
        }

        if (yMinEnabled) {
            posY = alignToOrigin ? (Math.abs(minimum.y) % yMinInterval) : 0
            context.color(yMinColor)
            context.lineWidth(yMinLineWidth)
            context.beginUnion()
            while (posY <= size.y) {
                context.lineFromTo(0.0, posY, 0.0,
                                   yVector.x, posY, yVector.z)
                posY += yMinInterval
            }
            context.endUnion()
        }

        if (zMinEnabled) {
            posZ = alignToOrigin ? (Math.abs(minimum.z) % zMinInterval) : 0
            context.color(zMinColor)
            context.lineWidth(zMinLineWidth)
            context.beginUnion()
            while (posZ <= size.z) {
                context.lineFromTo(0.0, 0.0, posZ,
                                   zVector.x, zVector.y, posZ)
                posZ += zMinInterval
            }
            context.endUnion()
        }

        if (xEnabled) {
            posX = alignToOrigin ? (Math.abs(minimum.x) % xInterval) : 0
            context.color(xColor)
            context.lineWidth(xLineWidth)
            context.beginUnion()
            while (posX <= size.x) {
                context.lineFromTo(posX, 0.0, 0.0,
                                   posX, xVector.y, xVector.z)
                posX += xInterval
            }
            context.endUnion()
        }

        if (yEnabled) {
            posY = alignToOrigin ? (Math.abs(minimum.y) % yInterval) : 0
            context.color(yColor)
            context.lineWidth(yLineWidth)
            context.beginUnion()
            while (posY <= size.y) {
                context.lineFromTo(0.0, posY, 0.0,
                                   yVector.x, posY, yVector.z)
                posY += yInterval
            }
            context.endUnion()
        }

        if (zEnabled) {
            posZ = alignToOrigin ? (Math.abs(minimum.z) % zInterval) : 0
            context.color(zColor)
            context.lineWidth(zLineWidth)
            context.beginUnion()
            while (posZ <= size.z) {
                context.lineFromTo(0.0, 0.0, posZ,
                                   zVector.x, zVector.y, posZ)
                posZ += zInterval
            }
            context.endUnion()
        }

        context.endUnion()
        context.update()
    }

    Component.onCompleted: {
        onMinimumChanged.connect(needsUpdate)
        onMaximumChanged.connect(needsUpdate)
        onLineWidthAxis1Changed.connect(needsUpdate)
        onLineWidthAxis2Changed.connect(needsUpdate)
        onLineWidthAxis1MinChanged.connect(needsUpdate)
        onLineWidthAxis2MinChanged.connect(needsUpdate)
        onColorAxis1Changed.connect(needsUpdate)
        onColorAxis2Changed.connect(needsUpdate)
        onColorAxis1MinChanged.connect(needsUpdate)
        onColorAxis2MinChanged.connect(needsUpdate)
        onIntervalAxis1Changed.connect(needsUpdate)
        onIntervalAxis2Changed.connect(needsUpdate)
        onIntervalAxis1MinChanged.connect(needsUpdate)
        onIntervalAxis2MinChanged.connect(needsUpdate)
        onEnableAxis1Changed.connect(needsUpdate)
        onEnableAxis2Changed.connect(needsUpdate)
        onEnableAxis1MinChanged.connect(needsUpdate)
        onEnableAxis2MinChanged.connect(needsUpdate)
        onAlignToOriginChanged.connect(needsUpdate)
        onPlaneChanged.connect(needsUpdate)
    }
}
