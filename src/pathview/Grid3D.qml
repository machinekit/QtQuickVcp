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
    property real lineWidth: 1.0
    property color color: "gray"
    property real intervalX: 0.5
    property real intervalY: 0.5

    id: root
    onPaint: {
        var size = Qt.vector3d(maximum.x - minimum.x, maximum.y - minimum.y, maximum.z - minimum.z)
        var posX = 0
        var posY = 0

        context.prepare(this)
        context.reset()
        context.color(root.color)
        context.lineWidth(root.lineWidth)
        context.translate(minimum)
        context.beginUnion()
        while (posX <= size.x) {
            context.lineFromTo(posX, 0.0, 0.0,
                               posX, size.y, 0.0)
            posX += intervalX
        }

        while (posY <= size.y) {
            context.lineFromTo(0.0, posY, 0.0,
                               size.x, posY, 0.0)
            posY += intervalY
        }
        context.endUnion()
        context.update()
    }

    Component.onCompleted: {
        onMinimumChanged.connect(needsUpdate)
        onMaximumChanged.connect(needsUpdate)
        onLineWidthChanged.connect(needsUpdate)
        onColorChanged.connect(needsUpdate)
        onIntervalXChanged.connect(needsUpdate)
        onIntervalYChanged.connect(needsUpdate)
    }
}
