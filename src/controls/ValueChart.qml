import QtQuick 2.0

Item {
    property color backgroundColor: "black"
    property color gridColor: "#222222"
    property color textColor: "black"
    property color signalColor: "red"
    property color hLineColor: "#666666"
    property color positiveChangeColor: "green"
    property color negativeChangeColor: "red"
    property int timeSpan: 5*60000    // timespan in ms
    property var valueModel: null
    property real startTimestamp: 0
    property real endTimestamp: 0
    property int minValue: 0
    property int maxValue: 300
    property double yGrid: 20
    property double xGrid: 1000
    property int signalLineWidth: 2
    property int gridLineWidth: 1
    property double changeScale: 10.0
    property bool changeGraphEnabled: true
    property bool autoScroll: true
    property double scrollZoomFactor: 0.1
    property alias leftTextVisible: leftText.visible
    property alias rightTextVisible: rightText.visible

    id: chart
    width: 320
    height: 320

    function update() {
        if (valueModel.ready && autoScroll)
        {
            chart.endTimestamp = valueModel.endTimestamp
        }
        chart.startTimestamp = endTimestamp - timeSpan

        canvas.requestPaint();
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

            if (endTimestamp == valueModel.endTimestamp)    // enable autoscrolling if someone moves to the end
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
            console.log(sign)
            timeSpan *= (1-sign*scrollZoomFactor)
            showMessage(qsTr("Timespan: ") + timeSpan + "ms")
        }
    }
    Text {
        id: leftText
        color: chart.textColor
        anchors.left: parent.left
        anchors.leftMargin: parent.width * 0.02
        anchors.top: parent.top
        text: qsTr("Value: <br>") + "<b>" + valueModel.currentValue.toFixed(2) + "</b>"
    }

    Text {
        id: rightText
        color: chart.textColor
        anchors.right: parent.right
        anchors.rightMargin: leftText.anchors.leftMargin
        anchors.top: parent.top
        text: qsTr("Target: <br>") + "<b>" + valueModel.targetValue.toFixed(2) + "</b>"
    }

    Text {
        id: messageText
        color: chart.textColor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
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

    function showMessage(message) {
      messageText.text = message
      messageText.visible = true
      messageTimer.running = true
    }

    Canvas {
        id: canvas
        width: parent.width
        anchors.top: rightText.bottom
        anchors.bottom: parent.bottom
        renderTarget: Canvas.FramebufferObject
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

            for (var i = minValue/yGrid; i < maxValue/yGrid; i += 1) {
                            var y = canvas.height-(i*yGrid-minValue)/(maxValue-minValue)*canvas.height;
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
                var y = change*(canvas.height/(maxValue-minValue))*changeScale;

                ctx.fillRect(x, Math.max(canvas.height-y, 0), canvas.width/points.length, canvas.height);
            }
            ctx.restore();
        }

        onPaint: {
            var ctx = canvas.getContext("2d");

            ctx.globalCompositeOperation = "source-over";
            ctx.lineWidth = 1;

            drawBackground(ctx);
            if (!valueModel.ready)
                return;

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
                                y: canvas.height-(item.value-minValue)/(maxValue-minValue)*canvas.height,
                                value: item.value
                            });
            }
            if (changeGraphEnabled)
                drawChange(ctx, first, last, positiveChangeColor, negativeChangeColor, points)
            drawHLine(ctx, canvas.height-(valueModel.targetValue-minValue)/(maxValue-minValue)*canvas.height, hLineColor)
            drawValue(ctx, first, last, signalColor, points)

        }
    }
}
