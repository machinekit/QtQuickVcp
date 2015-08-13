import QtQuick 2.0

Canvas {
    property bool animationRunning: false
    property color color: "red"
    property double value: 25.0
    property double minimumValue: 0.0
    property double maximumValue: 100.0
    property double defaultValue: (maximumValue - minimumValue) / 2
    property double stepSize: 0.1
    property bool readOnly: false
    property bool multicolor: false
    property string style: "Flat"

    property double factor: (2*Math.PI/(maximumValue - minimumValue))
    property double scale: 1 / (factor * stepSize)
    property double angle: (value-minimumValue) * factor - Math.PI/2
    property double spanAngle: (value - minimumValue) * factor
    property alias animating: animate.running

    property var chunkColors: multicolor ?
                                  [Qt.rgba(197,81,134,255), Qt.rgba(232,156,132,255), Qt.rgba(254,197,107,255), Qt.rgba(81,197,212,255)]
                                : []

    id: root
    contextType: "2d"
    smooth: true

    onAngleChanged: requestPaint()
    onColorChanged: requestPaint()

    onPaint:
    {
        if (!context)
            return

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

    MouseArea {
        id: events
        anchors.fill: parent
        enabled: !readOnly
        acceptedButtons: Qt.AllButtons

        function endValueFromPoint(x, y) {
            var theta = Math.atan2(x, -y)
            var angle = (theta + 2*Math.PI) % (2*Math.PI)
            var newValue = Math.round(angle*root.scale) * root.stepSize + minimumValue
            return Math.max(Math.min(newValue, maximumValue), minimumValue)
        }

        function calculateValue(x,y) {
            var dx = (x - height/2)
            var dy = (y - width/2)
            value = endValueFromPoint(dx,dy)
        }

        onWheel: {
            if (wheel.angleDelta.y < 0) {
                if (root.value < root.maximumValue) {
                    root.value += root.stepSize
                }
                else {
                    root.value = root.maximumValue
                }
            }
            else {
                if (root.value > root.minimumValue) {
                    root.value -= root.stepSize
                }
                else {
                    root.value = root.minimumValue
                }
            }
        }

        onClicked: {
            if (mouse.button == Qt.LeftButton) {
                calculateValue(mouse.x,mouse.y)
            }
            else
            {
                value = defaultValue
            }
        }
        onPositionChanged: calculateValue(mouseX,mouseY)
    }

    Behavior on value {
        enabled: !events.pressed
        SmoothedAnimation { id: animate; duration: 800 }
    }
}

