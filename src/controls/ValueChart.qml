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
import QtQuick.Controls 1.0
import Machinekit.Controls 1.0

/*!
    \qmltype ValueChart
    \inqmlmodule Machinekit.Controls
    \brief Provides a chart to display value data.
    \ingroup controls

    A value chart has to be used in combination with a value model to be useful.

    \qml
    ValueChart {
        id: valueChart
        valueModel: ValueModel {
                        id: myValueModel
                        name: "My value model"
                    }
    }
    \endqml

    \sa ValueModel, LogChart
*/
Item {
    /*! This property holds the value model connected with the chart.

        The default value is \c{null}.
    */
    property var valueModel: null

    /*! This property holds the background color of the chart.

        The default value is \c{black}.
    */
    property color backgroundColor: "black"

    /*! This property holds the color of the chart grid.

        The default value is \c{#222222}.
    */
    property color gridColor: "#222222"

    /*! This property holds color of the chart's texts.

        The default value is \c{black}.
    */
    property color textColor: "black"

    /*! This property holds the color of the displayed signal.

        The default value is \c{red}.
    */
    property color signalColor: "red"

    /*! This property holds the color of the horizontal target value line.

        The default value is \c{#666666}.
    */
    property color hLineColor: "#666666"

    /*! This property holds the color of positive changes.

        The default value is \c{green}.
    */
    property color positiveChangeColor: "green"

    /*! This property holds the color of negative changes.

        The default value is \c{red}.
    */
    property color negativeChangeColor: "red"

    /*! This property holds the time span displayed in the chart in ms.

        The default value is \c{300000}.
    */
    property int timeSpan: 5*60000    // timespan in ms

    /*! \internal */
    property real startTimestamp: 0

    /*! \internal */
    property real endTimestamp: 0

    /*! This property holds the minimum value that should be visible in the grid.

        The default value is \c{0}.
    */
    property double minimumValue: 0

    /*! This property holds the maximum value that should be visible in the grid.

        The default value is \c{0}.
    */
    property double maximumValue: 300

    /*! This property holds the resolution of the grid for the y axis.

        The default value is \c{20}.
    */
    property double yGrid: 20

    /*! This property holds the resolution of the grid for the x axis.

        The default value is \c{10000}.
    */
    property double xGrid: 10000

    /*! This property holds the width of signal.

        The default value is \c{2}.
    */
    property int signalLineWidth: 2

    /*! This property holds the width of the grid lines.

        The default value is \c{0}.
    */
    property int gridLineWidth: 1

    /*! This property holds the scaling value for the change graph.

        The default value is \c{10}.
    */
    property double changeGraphScale: 10.0

    /*! This property enables or disabled the change graph.

        The default value is \c{true}.
    */
    property bool changeGraphEnabled: true

    /*! This property holds wether auto scrolling should be enabled or not.

        The default value is \c{true}.
    */
    property bool autoScroll: true

    /*! This property holds the zoom factor for auto scrolling.

        The default value is \c{0.1}.
    */
    property double scrollZoomFactor: 0.1

    /*! \qmlproperty bool leftTextVisible

        This property holds wether the left text should be visible or not.

        The default value is \c{true}.
    */
    property alias leftTextVisible: leftText.visible

    /*! \qmlproperty bool rightTextVisible

        This property holds wether the right text should be visible or not.

        The default value is \c{true}.
    */
    property alias rightTextVisible: rightText.visible

    /*! This property indicates the amount of decimals.
      Note that if you enter more decimals than specified, they will
      be truncated to the specified amount of decimal places.

      The default value is \c{2}.
    */
    property int decimals: 2

    /*! The prefix for the value. I.e "$"
    */
    property string prefix: ""

    /*! The suffix for the value. I.e "cm"
    */
    property string suffix: ""

    id: chart
    width: 320
    height: 320
    implicitWidth: 320
    implicitHeight: 320

    /*! Updates and repaints the chart.
    */
    function update() {
        if (valueModel.ready && autoScroll)
        {
            chart.endTimestamp = valueModel.endTimestamp
        }
        chart.startTimestamp = endTimestamp - timeSpan

        canvas.requestPaint();
    }

    /*! \internal */
    function showMessage(message) {
      messageText.text = message
      messageText.visible = true
      messageTimer.running = true
    }

    MouseArea {
        property int pressStart: 0
        property real startEndTimestamp: 0

        id: mouseArea

        anchors.fill: parent
        onPressed: {
            pressStart = mouseX
            startEndTimestamp = endTimestamp
        }
        onReleased: {

        }
        onMouseXChanged: {
            endTimestamp = Math.min(startEndTimestamp + (pressStart-mouseX)/chart.width*timeSpan, valueModel.endTimestamp)

            if (endTimestamp === valueModel.endTimestamp)    // enable autoscrolling if someone moves to the end
            {
                autoScroll = true
                showMessage(qsTr("Autoscroll enabled"))
            }
            else
            {
                autoScroll = false
                showMessage(qsTr("Autoscroll disabled"))
            }
        }

        onWheel: {
            var sign = (wheel.angleDelta.y < 0) ? -1 : 1
            var tempTimeSpan = timeSpan * (1-sign*scrollZoomFactor)
            if (tempTimeSpan > 1000) {
                tempTimeSpan = Math.round(tempTimeSpan / 1000) * 1000
                showMessage(qsTr("Timespan: ") + (tempTimeSpan / 1000) + "s")
            }
            else {
                showMessage(qsTr("Timespan: ") + tempTimeSpan + "ms")
            }

            timeSpan = tempTimeSpan
        }
    }
    Label {
        id: leftText
        color: chart.textColor
        anchors.left: parent.left
        anchors.leftMargin: parent.width * 0.02
        anchors.top: parent.top
        text: qsTr("Value: <br>") + "<b>" + chart.prefix + ((valueModel != null) ? valueModel.currentValue.toFixed(chart.decimals) : 0) + chart.suffix + "</b>"
    }

    Label {
        id: rightText
        color: chart.textColor
        anchors.right: parent.right
        anchors.rightMargin: leftText.anchors.leftMargin
        anchors.top: parent.top
        height: (leftText.visible || rightText.visible) ? implicitHeight : 0    // hides top bar if no label visible
        text: qsTr("Target: <br>") + "<b>" + chart.prefix + ((valueModel != null) ? valueModel.targetValue.toFixed(chart.decimals) : 0) + chart.suffix + "</b>"
    }

    Label {
        id: messageText
        color: (rightText.height == 0) ? chart.signalColor : chart.textColor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        z: canvas.z + 1
        text: "<b>" + "blablabla" + "</b>"
        font.bold: true
        visible: false
    }

    Timer {
        id: messageTimer
        interval: 1500
        running: false
        repeat: false
        onTriggered: messageText.visible = false
    }

    Canvas {
        id: canvas

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: rightText.bottom
        anchors.bottom: parent.bottom
        antialiasing: true

        property int frames: first
        property int mouseX: 0
        property int mouseY: 0
        property int mousePressedX: 0
        property int mousePressedY: 0
        property int movedY: 0
        property real scaleX: 1.0
        property real scaleY: 1.0
        property int first: 0
        property int last: 0

        property int pixelSkip: 1

        function drawBackground(ctx) {
            ctx.save();
            ctx.fillStyle = chart.backgroundColor;
            ctx.fillRect(0, 0, canvas.width, canvas.height);
            ctx.strokeStyle = chart.gridColor;
            ctx.lineWidth = gridLineWidth
            ctx.beginPath();

            for (var i = minimumValue/yGrid; i < maximumValue/yGrid; i += 1) {
                            var y = canvas.height-(i*yGrid-minimumValue)/(maximumValue-minimumValue)*canvas.height;
                            ctx.moveTo(0, y);
                            ctx.lineTo(canvas.width, y);
                        }

            for (i = 0; i < timeSpan/xGrid; i += 1) {
                            var x = (i*xGrid)/(timeSpan)*canvas.width;
                            ctx.moveTo(x, 0);
                            ctx.lineTo(x, canvas.height);
                        }

            ctx.stroke();
            ctx.restore();
        }

        function drawValue(ctx, from, to, color, points)
        {
            ctx.save();
            ctx.globalAlpha = 1;
            ctx.strokeStyle = color;
            ctx.lineWidth = signalLineWidth;
            ctx.beginPath();

            var w = canvas.width/points.length;
            var end = points.length;
            for (var i = 0; i < end; i+=pixelSkip) {
                var x = points[i].x;
                var y = points[i].y;
                if (i == 0) {
                    ctx.moveTo(x+w/2, y);
                } else {
                    ctx.lineTo(x+w/2, y);
                }
            }
            ctx.stroke();
            ctx.restore();
        }

        function drawHLine(ctx, y, color)
        {
            ctx.save();
            ctx.globalAlpha = 1;
            ctx.strokeStyle = color;
            ctx.lineWidth = signalLineWidth;
            ctx.beginPath();

            ctx.moveTo(0, y);
            ctx.lineTo(canvas.width, y)

            ctx.stroke();
            ctx.restore();
        }

        function drawKLine(ctx, from, to, points, highest)
        {
            ctx.save();
            ctx.globalAlpha = 0.4;
            ctx.lineWidth = 2;
            var end = points.length;
            for (var i = 0; i < end; i+=pixelSkip) {
                var x = points[i].x;
                var open = canvas.height * points[i].open/highest - canvas.movedY;
                var close = canvas.height * points[i].close/highest - canvas.movedY;
                var high = canvas.height * points[i].high/highest - canvas.movedY;
                var low = canvas.height * points[i].low/highest - canvas.movedY;

                var top, bottom;
                if (close <= open) {
                    ctx.fillStyle = Qt.rgba(1, 0, 0, 1);
                    ctx.strokeStyle = Qt.rgba(1, 0, 0, 1);
                    top = close;
                    bottom = open;
                } else {
                    ctx.fillStyle = Qt.rgba(0, 1, 0, 1);
                    ctx.strokeStyle = Qt.rgba(0, 1, 0, 1);
                    top = open;
                    bottom = close;
                }

                var w1, w2;
                w1 = canvas.width/points.length;
                w2 = w1 > 10 ? w1/2 : w1;

                ctx.fillRect(x + (w1 - w2)/2, top, w2, bottom - top);
                ctx.beginPath();
                ctx.moveTo(x+w1/2, high);
                ctx.lineTo(x+w1/2, low);
                ctx.stroke();
            }
            ctx.restore();
        }

        function drawChange(ctx, from, to, color1, color2, points)
        {
            ctx.save();
            ctx.globalAlpha = 0.2;
            ctx.strokeStyle = Qt.rgba(0.8, 0.8, 0.8, 1);
            ctx.lineWidth = 1;

            var end = points.length;
            for (var i = 1; i < end; i+=pixelSkip) {
                var change = points[i].value - points[i-1].value

                if (change >= 0)
                {
                    ctx.fillStyle = color1;
                }
                else
                {
                    ctx.fillStyle = color2;
                }

                change = Math.abs(change)

                var x = points[i].x;
                var y = change*(canvas.height/(maximumValue-minimumValue))*changeGraphScale;

                ctx.fillRect(x, Math.max(canvas.height-y, 0), canvas.width/points.length, canvas.height);
            }
            ctx.restore();
        }

        onPaint: {
            var ctx = canvas.getContext("2d");

            ctx.globalCompositeOperation = "source-over";
            ctx.lineWidth = 1;

            drawBackground(ctx);

            if (!valueModel.ready) {
                return;
            }

            last = valueModel.indexOf(chart.endTimestamp)
            first = valueModel.indexOf(chart.startTimestamp)
            first = Math.max(first, 0);

            var highestValue = valueModel.highestValue;
            var lowestValue = valueModel.lowestValue;
            var points = [];
            for (var i = 0; i <= last - first; i+=pixelSkip) {
                var item = valueModel.get(i+first);
                if (item === undefined)
                    continue
                points.push({
                                x: (item.timestamp-startTimestamp)*canvas.width/(endTimestamp-startTimestamp+1),
                                y: canvas.height-(item.value-minimumValue)/(maximumValue-minimumValue)*canvas.height,
                                value: item.value
                            });
            }
            if (changeGraphEnabled)
                drawChange(ctx, first, last, positiveChangeColor, negativeChangeColor, points)
            drawHLine(ctx, canvas.height-(valueModel.targetValue-minimumValue)/(maximumValue-minimumValue)*canvas.height, hLineColor)
            drawValue(ctx, first, last, signalColor, points)
        }
    }
}
