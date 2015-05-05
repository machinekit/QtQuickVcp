import QtQuick 2.0

Canvas {
    property bool animationRunning: false
    property color color: "red"
    property double value: 25.0
    property double minimumValue: 0.0
    property double maximumValue: 100.0
    property double stepSize: 0.1
    property bool readOnly: false
    property bool multicolor: false
    property string style: "Flat"

    property double factor: (2*Math.PI/(maximumValue - minimumValue))
    property double scale: 1 / (factor * stepSize)
    property double angle: value * factor - Math.PI/2
    property double spanAngle: (value - minimumValue) * factor

    property var chunkColors: multicolor ?
                                  [Qt.rgba(197,81,134,255), Qt.rgba(232,156,132,255), Qt.rgba(254,197,107,255), Qt.rgba(81,197,212,255)]
                                : []

    function endValueFromPoint(x, y) {
        var theta = Math.atan2(x, -y)
        var angle = (theta + 2*Math.PI) % (2*Math.PI)
        return Math.round(angle*scale) * stepSize
    }

    id: root
    contextType: "2d"
    smooth: true

    onAngleChanged: requestPaint()
    onSpanAngleChanged: requestPaint()
    onColorChanged: requestPaint()

    onPaint:
    {
        var w = (width/3.0) - 1.0
        var m = 1.5
        context.reset()
        if (style == "Flat")
        {
            if (!multicolor)
            {
                context.beginPath()
                context.fillStyle = root.color
                context.arc(width/2, height/2, width/2-m, -Math.PI/2, angle)
                context.lineTo(width/2, height/2)
                context.fill()
            }
        }
    }

    width: 200
    height: 200
}

