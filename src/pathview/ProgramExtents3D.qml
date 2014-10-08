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
    property int axes: 3
    property vector3d maximum: Qt.vector3d(2.0, 3.0, 2.5)
    property vector3d minimum: Qt.vector3d(1.0, 0.8, 0.0)
    readonly property vector3d size: maximum.minus(minimum)
    readonly property vector3d center: minimum.plus(maximum).times(0.5)
    readonly property bool valid: minimum != maximum
    property vector3d limitMaximum: Qt.vector3d(1.5, 3.0, 2.5)
    property vector3d limitMinimum: Qt.vector3d(0.0, 0.0, 0.0)
    property color color: "magenta"
    property color limitColor: "red"
    property real textSize: 0.5
    property string prefix: ""
    property string suffix: ""
    property int decimals: 2
    property string viewMode: "Perspective"

    id: root
    onPaint: {
        var lineEnding = root.textSize/2.0
        var lineOffset = root.textSize
        var textOffset = root.textSize/4.0

        var xAxisVisible = true
        var yAxisVisible = true
        var zAxisVisible = true
        var xAxisRotation = 0.0
        var yAxisRotation = 0.0
        var zAxisRotation = 0.0
        var xAxisOffset = Qt.vector3d(0,0,0)
        var yAxisOffset = Qt.vector3d(0,0,0)
        var zAxisOffset = Qt.vector3d(0,0,0)

        switch (root.viewMode) {
        case "Top":
            zAxisVisible = false
            xAxisOffset = Qt.vector3d(root.minimum.x, root.minimum.y - lineOffset, root.minimum.z)
            yAxisOffset = Qt.vector3d(root.minimum.x - lineOffset, root.minimum.y, root.minimum.z)
            break
        case "RotatedTop":
            zAxisVisible = false
            yAxisRotation = 180
            xAxisOffset = Qt.vector3d(root.minimum.x, root.minimum.y - lineOffset, root.minimum.z)
            yAxisOffset = Qt.vector3d(root.maximum.x + lineOffset, root.minimum.y, root.minimum.z)
            break
        case "Front":
            yAxisVisible = false
            xAxisRotation = 90
            xAxisOffset = Qt.vector3d(root.minimum.x, 0.0, root.minimum.z - lineOffset)
            zAxisOffset = Qt.vector3d(root.minimum.x - lineOffset, root.minimum.y - lineOffset, root.minimum.z)
            break
        case "Side":
            xAxisRotation = false
            yAxisRotation = -90
            zAxisRotation = 90
            yAxisOffset = Qt.vector3d(0.0, root.minimum.y, root.minimum.z - lineOffset)
            zAxisOffset = Qt.vector3d(root.minimum.x - lineOffset, root.minimum.y - lineOffset, root.minimum.z)
            break
        case "Perspective":
            xAxisOffset = Qt.vector3d(root.minimum.x, root.minimum.y - lineOffset, root.minimum.z)
            yAxisOffset = Qt.vector3d(root.minimum.x - lineOffset, root.minimum.y, root.minimum.z)
            zAxisOffset = Qt.vector3d(root.minimum.x - lineOffset, root.minimum.y - lineOffset, root.minimum.z)
        }

        context.prepare(this)
        context.reset()

        if (!valid) {
            context.update()
            return
        }

        context.color(root.color)
        context.beginUnion()

            if (xAxisVisible)
            {
                context.translate(xAxisOffset)
                context.rotate(xAxisRotation, 1.0, 0.0, 0.0)
                context.beginUnion()
                    context.translate(0.0, lineEnding/2.0, 0.0)
                    context.line(0.0, -lineEnding, 0.0)
                    context.line(root.size.x, 0.0, 0.0)
                    context.translate(root.size.x, lineEnding/2.0, 0.0)
                    context.line(0.0, -lineEnding, 0.0)

                    context.translate(root.size.x/2.0, -textOffset - root.textSize, 0.0)
                    context.scale(root.textSize, root.textSize, root.textSize)
                    context.text(root.prefix + root.size.x.toFixed(root.decimals) + root.suffix, GLView3D.AlignCenter)

                    if (root.limitMaximum.x < root.maximum.x) {
                        context.color(root.limitColor)
                    }
                    context.translate(root.size.x + root.textSize/2.0, -textOffset - lineEnding/2.0, 0.0)
                    context.scale(root.textSize, root.textSize, root.textSize)
                    context.rotate(90, 0.0, 0.0, 1.0)
                    context.text(root.prefix + (root.minimum.x + root.size.x).toFixed(root.decimals) + root.suffix, GLView3D.AlignRight)

                    if (root.limitMinimum.x > root.minimum.x) {
                        context.color(root.limitColor)
                    }
                    context.translate(root.textSize/2.0, -textOffset - lineEnding/2.0, 0.0)
                    context.scale(root.textSize, root.textSize, root.textSize)
                    context.rotate(90, 0.0, 0.0, 1.0)
                    context.text(root.prefix + root.minimum.x.toFixed(root.decimals) + root.suffix, GLView3D.AlignRight)
                context.endUnion()
            }

            if ((axes > 1) && (yAxisVisible))
            {
                context.translate(yAxisOffset)
                context.rotate(yAxisRotation, 0.0, 1.0, 0.0)
                context.rotate(90, 0.0, 0.0, 1.0)
                context.beginUnion()
                    context.translate(0.0, lineEnding/2.0, 0.0)
                    context.line(0.0, -lineEnding, 0.0)
                    context.line(root.size.y, 0.0, 0.0)
                    context.translate(root.size.y, lineEnding/2.0, 0.0)
                    context.line(0.0, -lineEnding, 0.0)

                    context.translate(root.size.y/2.0, textOffset + root.textSize, 0.0)
                    context.rotate(180, 0.0, 0.0, 1.0)
                    context.scale(root.textSize, root.textSize, root.textSize)
                    context.text(root.prefix + root.size.y.toFixed(root.decimals) + root.suffix, GLView3D.AlignCenter)

                    if (root.limitMaximum.y < root.maximum.y) {
                        context.color(root.limitColor)
                    }
                    context.translate(root.size.y - root.textSize/2.0, textOffset + lineEnding/2.0, 0.0)
                    context.scale(root.textSize, root.textSize, root.textSize)
                    context.rotate(-90, 0.0, 0.0, 1.0)
                    context.text(root.prefix + (root.minimum.y + root.size.y).toFixed(root.decimals) + root.suffix, GLView3D.AlignRight)

                    if (root.limitMinimum.y > root.minimum.y) {
                        context.color(root.limitColor)
                    }
                    context.translate(-root.textSize/2.0, textOffset + lineEnding/2.0, 0.0)
                    context.scale(root.textSize, root.textSize, root.textSize)
                    context.rotate(-90, 0.0, 0.0, 1.0)
                    context.text(root.prefix + root.minimum.y.toFixed(root.decimals) + root.suffix, GLView3D.AlignRight)
                context.endUnion()
            }

            if ((axes > 2) && zAxisVisible)
            {

                context.translate(zAxisOffset)
                context.rotate(zAxisRotation, 0.0, 0.0, 1.0)
                context.rotate(-90, 0.0, 1.0, 0.0)
                context.rotate(-90, 1.0, 0.0, 0.0)
                context.beginUnion()
                    context.translate(0.0, lineEnding/2.0, 0.0)
                    context.line(0.0, -lineEnding, 0.0)
                    context.line(root.size.z, 0.0, 0.0)
                    context.translate(root.size.z, lineEnding/2.0, 0.0)
                    context.line(0.0, -lineEnding, 0.0)

                    context.translate(root.size.z/2.0, -textOffset - root.textSize, 0.0)
                    context.scale(root.textSize, root.textSize, root.textSize)
                    context.text(root.prefix + root.size.z.toFixed(root.decimals) + root.suffix, GLView3D.AlignCenter)

                    if (root.limitMaximum.z < root.maximum.z) {
                        context.color(root.limitColor)
                    }
                    context.translate(root.size.z - root.textSize/2.0, -textOffset - lineEnding/2.0, 0.0)
                    context.scale(root.textSize, root.textSize, root.textSize)
                    context.rotate(-90, 0.0, 0.0, 1.0)
                    context.text(root.prefix + (root.minimum.z + root.size.z).toFixed(root.decimals) + root.suffix, GLView3D.AlignLeft)

                    if (root.limitMinimum.z > root.minimum.z) {
                        context.color(root.limitColor)
                    }
                    context.translate(-root.textSize/2.0, -textOffset - lineEnding/2.0, 0.0)
                    context.scale(root.textSize, root.textSize, root.textSize)
                    context.rotate(-90, 0.0, 0.0, 1.0)
                    context.text(root.prefix + root.minimum.z.toFixed(root.decimals) + root.suffix, GLView3D.AlignLeft)
                context.endUnion()
            }

        context.endUnion()
        context.update()
    }

    Component.onCompleted: {
        onAxesChanged.connect(needsUpdate)
        onMaximumChanged.connect(needsUpdate)
        onMinimumChanged.connect(needsUpdate)
        onColorChanged.connect(needsUpdate)
        onTextSizeChanged.connect(needsUpdate)
        onPrefixChanged.connect(needsUpdate)
        onSuffixChanged.connect(needsUpdate)
        onDecimalsChanged.connect(needsUpdate)
        onViewModeChanged.connect(needsUpdate)
    }
}
