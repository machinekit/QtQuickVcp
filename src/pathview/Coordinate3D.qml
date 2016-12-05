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
    property real axesLength: 5.0
    property real textSize: 1.0
    property real lineWidth: 1.0
    property color xAxisColor: "red"
    property color yAxisColor: "green"
    property color zAxisColor: "blue"
    property real xAxisRotation: 0.0
    property real yAxisRotation: 0.0
    property real zAxisRotation: 0.0

    id: root
    onPaint: {
        context.prepare(this);
        context.reset();
        context.lineWidth(root.lineWidth)
        context.beginUnion();

        context.color(root.xAxisColor);
        context.rotate(xAxisRotation, 1.0, 0.0, 0.0);
        context.beginUnion();
        context.line(root.axesLength, 0.0, 0.0);
        context.translate(root.axesLength, -root.textSize/2.0, 0.0);
        context.scale(root.textSize, root.textSize, root.textSize);
        context.text("X");
        context.endUnion();

        if (axes > 1) {
            context.color(root.yAxisColor);
            context.rotate(yAxisRotation, 0.0, 1.0, 0.0);
            context.beginUnion();
            context.line(0.0, root.axesLength, 0.0);
            context.translate(root.textSize/2.0, root.axesLength, 0.0);
            context.rotate(90, 0.0, 0.0, 1.0);
            context.scale(root.textSize, root.textSize, root.textSize);
            context.text("Y");
            context.endUnion();
        }

        if (axes > 2) {
            context.color(root.zAxisColor);
            context.rotate(zAxisRotation, 0.0, 0.0, 1.0);
            context.beginUnion();
            context.line(0.0, 0.0, root.axesLength);
            context.translate(0.0, 0.0, root.axesLength);
            context.rotate(90, 1.0, 0.0, 0.0);
            context.scale(root.textSize, root.textSize, root.textSize);
            context.text("Z", GLView3D.AlignCenter);
            context.endUnion();
        }

        context.endUnion();
        context.update();
    }

    Component.onCompleted: {
        onAxesChanged.connect(needsUpdate);
        onAxesLengthChanged.connect(needsUpdate);
        onTextSizeChanged.connect(needsUpdate);
        onXAxisColorChanged.connect(needsUpdate);
        onYAxisColorChanged.connect(needsUpdate);
        onZAxisColorChanged.connect(needsUpdate);
        onXAxisRotationChanged.connect(needsUpdate);
        onYAxisRotationChanged.connect(needsUpdate);
        onZAxisRotationChanged.connect(needsUpdate);
    }
}
