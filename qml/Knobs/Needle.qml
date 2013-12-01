import QtQuick 2.0
import NeedleStyle 1.0

Item {
    id: item

    property int clickedEndValue: 0
    property int endValue : dialEndValue
    property int pointStyle : dialPointStyle

    Binding { target: indicator; property: "text"; value: endValue }

    NeedleStyle {
        id: chunk
        anchors.fill: parent
        x: parent.width/2
        y: parent.height/2 - height/2
        width: parent.width/2
        height: 30
        color: chunkColor
        style: pointStyle

        transform: Rotation {
            id: secondRotation
            origin.x: chunk.width/2.0
            origin.y: chunk.height/2.0
            angle: (endValue * 3.6) - 90.0
        }
    }

    MouseArea {
        anchors.fill: parent
        property int previousPosition: 0

        onWheel: {
            wheel.angleDelta.y < 0 ? endValue < 100 ? ++endValue : 100 : endValue > 0 ? --endValue : 0
        }

        onClicked: {
            var dx = (mouseX - parent.height/2);
            var dy = (mouseY - parent.width/2);
            clickedEndValue = chunk.endValueFromPoint(dx,dy);
            animate.start();
        }

        onPositionChanged: {
            var dx = (mouseX - parent.height/2);
            var dy = (mouseY - parent.width/2);
            endValue = chunk.endValueFromPoint(dx,dy);
        }
    }

        SmoothedAnimation {
            id: animate;
            target: item; property: "endValue";
            to:clickedEndValue;
            duration: 2000;
        }
}
