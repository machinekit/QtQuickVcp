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
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import Machinekit.Application 1.0

AbstractDigitalReadOut {
    property alias textColor: dummyLabel.color
    property alias font: dummyLabel.font

    id: droRect
    implicitWidth: positionLayout.width
                   + dtgLayout.visible * dtgLayout.width
                   + (offsetsVisible ? Screen.pixelDensity * 8 : Screen.pixelDensity * 3)
    implicitHeight: positionLayout.height
                    + extraLayout.visible * extraLayout.height
                    + g5xLayout.visible * g5xLayout.height
                    + g92Layout.visible * g92Layout.height
                    + (offsetsVisible ? Screen.pixelDensity * 8 : Screen.pixelDensity * 3)
    width: implicitWidth
    height: implicitHeight
    enabled: _ready

    Label {
        id: dummyLabel
        font.bold: true
        visible: false
    }

    Component {
        id: textLine
        Row {
            property double value: 0.12345
            property string title: "X:"
            property string type: "DTG"
            property bool homed: true

            id: root

            Label {
                id: typeLabel
                width: dummyLabel.font.pixelSize * 2

                color: dummyLabel.color
                font: dummyLabel.font
                text: root.type

                Loader {
                    sourceComponent: homedSymbol
                    anchors.top: typeLabel.top
                    anchors.bottom: typeLabel.bottom
                    //anchors.horizontalCenter: typeLabel.horizontalCenter
                    anchors.left: typeLabel.left
                    width: height
                    onLoaded: {
                        item.color = Qt.binding(function(){ return dummyLabel.color; });
                    }
                    active: type === ""
                    visible: root.homed
                }
            }
            Label {
                width: dummyLabel.font.pixelSize * 1.4

                color: dummyLabel.color
                font: dummyLabel.font
                text: root.title
                horizontalAlignment: Text.AlignRight
            }
            Label {
                width: dummyLabel.font.pixelSize * 6

                color: dummyLabel.color
                font: dummyLabel.font
                text: droRect.prefix + root.value.toFixed(droRect.decimals) + droRect.suffix
                horizontalAlignment: Text.AlignRight
            }
        }
    }

    Component {
        id: homedSymbol
        Canvas {
            property color color: "black"
            id: canvas
            contextType: "2D"
            width: 100
            height: 100
            onPaint: {
                var ctx = canvas.getContext("2d");
                var radius = Math.ceil(canvas.width/2 * 0.6);
                var size = canvas.width/2;

                ctx.save();
                ctx.clearRect(0,0,canvas.width, canvas.height);
                ctx.strokeStyle = color;
                ctx.lineWidth = 1;
                ctx.fillStyle = color;

                ctx.beginPath();
                ctx.moveTo(size-radius,size);
                ctx.arcTo(size-radius,size-radius,size,size-radius,radius);
                ctx.lineTo(canvas.width/2,canvas.height/2);    // right side
                ctx.closePath();
                ctx.stroke();

                ctx.beginPath();
                ctx.moveTo(size+radius,size);
                ctx.arcTo(size+radius,size+radius,size,size+radius,radius);
                ctx.lineTo(canvas.width/2,canvas.height/2);    // right side
                ctx.closePath();
                ctx.stroke();

                ctx.beginPath();
                ctx.moveTo(size,size-radius);
                ctx.arcTo(size+radius,size-radius,size+radius,size,radius);
                ctx.lineTo(canvas.width/2,canvas.height/2);    // right side
                ctx.closePath();
                ctx.stroke();
                ctx.fill();

                ctx.beginPath();
                ctx.moveTo(size,size+radius);
                ctx.arcTo(size-radius,size+radius,size-radius,size,radius);
                ctx.lineTo(canvas.width/2,canvas.height/2);    // right side
                ctx.closePath();
                ctx.stroke();
                ctx.fill();

                ctx.beginPath();
                ctx.moveTo(size, 0);
                ctx.lineTo(size, size*2);
                ctx.closePath();
                ctx.stroke();

                ctx.beginPath();
                ctx.moveTo(0,size);
                ctx.lineTo(size*2, size);
                ctx.closePath();
                ctx.stroke();

                ctx.restore();
            }
            onColorChanged: requestPaint()
        }
    }

    ColumnLayout {
        id: positionLayout
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: Screen.pixelDensity
        spacing: Screen.pixelDensity * 0.7

        Loader {
            sourceComponent: textLine
            active: droRect.lathe
            visible: active
            onLoaded: {
                item.title = qsTr("Rad:");
                item.type = "";
                item.value = Qt.binding(function(){ return Number(droRect.position['x']); });
                item.homed = Qt.binding(function(){ return droRect.axisHomed[0].homed; });
            }
        }

        Loader {
            sourceComponent: textLine
            active: droRect.lathe
            visible: active
            onLoaded: {
                item.title = qsTr("Dia:");
                item.type = "";
                item.value = Qt.binding(function(){ return Number(droRect.position['x']) * 2.0; });
                item.homed = false;
            }
        }

        Repeater {
            model: droRect.axes - Number(droRect.lathe)
            Loader {
                readonly property int pos: index + Number(droRect.lathe)
                sourceComponent: textLine
                onLoaded: {
                    item.title = Qt.binding(function(){ return (droRect.axisNames[pos] + ":"); });
                    item.type = "";
                    item.value = Qt.binding(function(){ return Number(droRect.position[droRect._axisNames[pos]]); });
                    item.homed = Qt.binding(function(){ return ((pos < droRect.axisHomed.length) && droRect.axisHomed[droRect._axisIndices[pos]].homed); });
                }
            }
        }
    }

    ColumnLayout {
        id: extraLayout
        anchors.left: parent.left
        anchors.top: positionLayout.bottom
        anchors.leftMargin: Screen.pixelDensity
        anchors.topMargin: Screen.pixelDensity * 0.7
        spacing: Screen.pixelDensity * 0.7
        visible: !droRect.offsetsVisible

        Loader {
            sourceComponent: textLine
            onLoaded: {
                item.title = qsTr("Vel:");
                item.type = "";
                item.homed = false;
                item.value = Qt.binding(function(){ return droRect.velocity; });
            }
            active: true
            visible: !droRect.offsetsVisible && velocityVisible
        }

        Loader {
            sourceComponent: textLine
            onLoaded: {
                item.title = qsTr("DTG:");
                item.type = "";
                item.homed = false;
                item.value = Qt.binding(function(){ return droRect.distanceToGo; });
            }
            active: true
            visible: !droRect.offsetsVisible && distanceToGoVisible
        }
    }

    ColumnLayout {
        id: dtgLayout
        anchors.left: positionLayout.right
        anchors.top: parent.top
        anchors.topMargin: Screen.pixelDensity
        anchors.leftMargin: Screen.pixelDensity * 5
        spacing: Screen.pixelDensity * 0.7
        visible: droRect.offsetsVisible

        Loader {
            id: radiusDtg
            sourceComponent: textLine
            active: droRect.lathe
            visible: active
            onLoaded: {
                item.title = "R:";
                item.type = qsTr("DTG");
                item.value = Qt.binding(function(){ return Number(droRect.dtg['x']); });
            }
        }

        Repeater {
            model: droRect.axes - Number(droRect.lathe)
            Loader {
                readonly property int pos: index + Number(droRect.lathe)
                sourceComponent: textLine
                onLoaded: {
                    item.title = Qt.binding(function(){ return droRect.axisNames[pos] + ":"; });
                    item.type = qsTr("DTG");
                    item.value = Qt.binding(function(){ return Number(droRect.dtg[droRect._axisNames[pos]]); });
                }
            }
        }

        Item {
            height: radiusDtg.height
            visible: radiusDtg.visible
        }
    }

    ColumnLayout {
        id: g5xLayout
        anchors.left: parent.left
        anchors.top: positionLayout.bottom
        anchors.leftMargin: Screen.pixelDensity
        anchors.topMargin: Screen.pixelDensity * 3
        spacing: Screen.pixelDensity * 0.7
        visible: droRect.offsetsVisible

        Repeater {
            model: droRect.axes
            Loader {
                sourceComponent: textLine
                onLoaded: {
                    item.title = Qt.binding(function(){ return droRect.axisNames[index] + ":"; });
                    item.type = Qt.binding(function(){ return droRect.g5xNames[droRect.g5xIndex-1]; });
                    item.value = Qt.binding(function(){ return Number(droRect.g5xOffset[droRect._axisNames[index]]); });
                }
            }
        }

        Loader {
            sourceComponent: textLine
            onLoaded: {
                item.title = "R:";
                item.type = Qt.binding(function(){ return droRect.g5xNames[droRect.g5xIndex-1]; });
                item.value = Qt.binding(function(){ return Number(droRect.rotationXy); });
            }
        }
    }

    ColumnLayout {
        id: g92Layout
        anchors.left: g5xLayout.right
        anchors.top: dtgLayout.bottom
        anchors.leftMargin: Screen.pixelDensity * 5
        anchors.topMargin: Screen.pixelDensity * 3
        spacing: Screen.pixelDensity * 0.7
        visible: droRect.offsetsVisible

        Repeater {
            model: droRect.axes
            Loader {
                sourceComponent: textLine
                onLoaded: {
                    item.title = Qt.binding(function(){ return droRect.axisNames[index] + ":"; });
                    item.type = "G92";
                    item.value = Qt.binding(function(){ return Number(droRect.g92Offset[droRect._axisNames[index]]); });
                }
            }
        }
    }

    ColumnLayout {
        id: tloLayout
        anchors.left: parent.left
        anchors.top: g5xLayout.bottom
        anchors.leftMargin: Screen.pixelDensity
        anchors.topMargin: Screen.pixelDensity * 3
        spacing: Screen.pixelDensity * 0.7
        visible: droRect.offsetsVisible

        Repeater {
            model: droRect.axes
            Loader {
                sourceComponent: textLine
                onLoaded: {
                    item.title = Qt.binding(function(){ return droRect.axisNames[index] + ":"});
                    item.type = qsTr("TLO");
                    item.value = Qt.binding(function(){ return Number(droRect.toolOffset[droRect._axisNames[index]]); });
                }
            }
        }
    }
}
