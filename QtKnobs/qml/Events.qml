import QtQuick 2.0

MouseArea {
    anchors.fill: parent
    enabled: !readOnly

    function calculateValue(x,y) {
        var dx = (x - parent.height/2)
        var dy = (y - parent.width/2)
        if(mode===10)
            value = chunk.endValueFromPoint(dx,dy)
        else
            percent = chunk.endValueFromPoint(dx,dy)
    }

    onWheel: {
        if(mode===10)
            wheel.angleDelta.y < 0 ? value < maxValue ? ++value : maxValue : value > 0 ? --value : 0
        else
            wheel.angleDelta.y < 0 ? percent < 100 ? ++percent : 100 : percent > 0 ? --percent : 0
    }

    onClicked: calculateValue(mouseX,mouseY)
    onPositionChanged: calculateValue(mouseX,mouseY)
}
