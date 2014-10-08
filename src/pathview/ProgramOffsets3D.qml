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
    property real textSize: 14
    property color color: "cyan"
    property var g5xNames: ["G54", "G55", "G56", "G57", "G58", "G59", "G59.1", "G59.2", "G59.3"]
    property int g5xIndex: 1
    property var g5xOffset: {"x":0.12345, "y":0.234,"z":123.12,"a":324.3}
    property var g92Offset: {"x":0.12345, "y":0.234,"z":123.12,"a":324.3}

    id: programOffsets

    onPaint: {
        context.prepare(this)
        context.reset()

        context.color(color)
        context.lineWidth(1.0)
        context.beginUnion()

        if ((g5xOffset.x !== 0.0) || (g5xOffset.y !== 0.0) || (g5xOffset.z !== 0.0))
        {
            context.translate(0.0, 0.0, -textSize)
            context.rotate(90, 1.0, 0.0, 0.0)
            context.scale(textSize, textSize, textSize)
            if (g5xOffset.x < 0) {
                context.text(g5xNames[g5xIndex-1], GLView3D.AlignRight)
            } else {
                context.text(g5xNames[g5xIndex-1], GLView3D.AlignLeft)
            }
            context.lineFromTo(0.0, 0.0, 0.0,
                               g5xOffset.x, g5xOffset.y, g5xOffset.z)
        }

        if ((g92Offset.x !== 0.0) || (g92Offset.y !== 0.0) || (g92Offset.z !== 0.0))
        {
            context.translate(g5xOffset.x, g5xOffset.y, g5xOffset.z)
            context.rotate(90, 1.0, 0.0, 0.0)
            context.beginUnion()
            context.scale(textSize, textSize, textSize)
            if (g92Offset.x < 0) {
                context.text("G92", GLView3D.AlignRight)
            } else {
                context.text("G92", GLView3D.AlignLeft)
            }
            context.lineTo(g92Offset.x, g92Offset.y, g92Offset.z)
            context.endUnion()
        }

        context.endUnion()

        context.update()
    }

    Component.onCompleted: {
        onTextSizeChanged.connect(needsUpdate)
        onColorChanged.connect(needsUpdate)
        onG5xNamesChanged.connect(needsUpdate)
        onG5xIndexChanged.connect(needsUpdate)
        onG5xOffsetChanged.connect(needsUpdate)
        onG92OffsetChanged.connect(needsUpdate)
    }
}
