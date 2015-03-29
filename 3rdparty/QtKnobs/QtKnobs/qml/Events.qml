import QtQuick 2.0

MouseArea {
    anchors.fill: parent
    enabled: !readOnly

    function calculateValue(x,y) {
        var dx = (x - parent.height/2)
        var dy = (y - parent.width/2)
        value = chunk.endValueFromPoint(dx,dy)
    }

    onWheel: {
        wheel.angleDelta.y < 0 ? value < maxValue ? value += stepSize : maxValue : value > minValue ? value -= stepSize : minValue
    }

    onClicked: calculateValue(mouseX,mouseY)
    onPositionChanged: calculateValue(mouseX,mouseY)
}
