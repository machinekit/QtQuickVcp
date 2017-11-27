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
    property vector3d minimum: Qt.vector3d(0.0, 0.0, 0.0)
    property vector3d maximum: Qt.vector3d(10.0, 10.0, 10.0)
    readonly property vector3d size: maximum.minus(minimum)
    readonly property vector3d center: minimum.plus(maximum).times(0.5)
    property color color: "red"
    property real lineWidth: 1.0
    property bool lineStipple: true
    property real lineStippleLength: (size.length()  / 20.0)

    id: root

    onPaint: {
        var size = Qt.vector3d(maximum.x - minimum.x, maximum.y - minimum.y, maximum.z - minimum.z);
        context.prepare(this);
        context.reset();
        context.lineStipple(root.lineStipple, root.lineStippleLength);
        context.lineWidth(root.lineWidth);
        context.color(root.color);
        context.translate(minimum);
        context.beginUnion();
        context.lineTo(size.x, 0.0, 0.0);
        context.lineTo(size.x, size.y, 0.0);
        context.lineTo(0.0, size.y, 0.0);
        context.lineTo(0.0, 0.0, 0.0);
        if (axes > 2) {
            context.lineTo(0.0, 0.0, size.z);
            context.lineTo(size.x, 0.0, size.z);
            context.lineTo(size.x, size.y, size.z);
            context.lineTo(0.0, size.y, size.z);
            context.lineTo(0.0, 0.0, size.z);
            context.resetTransformations();
            context.lineFromTo(0.0, size.y, 0.0,
                               0.0, size.y, size.z);
            context.lineFromTo(size.x, size.y, 0.0,
                               size.x, size.y, size.z);
            context.lineFromTo(size.x, 0.0, 0.0,
                               size.x, 0.0, size.z);
        }
        context.endUnion();
        context.update();
    }

    Component.onCompleted: {
        onAxesChanged.connect(needsUpdate);
        onMinimumChanged.connect(needsUpdate);
        onMaximumChanged.connect(needsUpdate);
        onColorChanged.connect(needsUpdate);
        onLineWidthChanged.connect(needsUpdate);
        onLineStippleChanged.connect(needsUpdate);
        onLineStippleLengthChanged.connect(needsUpdate);
    }
}
