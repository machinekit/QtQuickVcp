import QtQuick 2.0
import ArcStyle 1.0

ArcStyle {
    id: chunk
    anchors.fill: parent
    endValue: dialEndValue
    color: chunkColor

    property int clickedEndValue

    Binding { target: indicator; property: "text"; value: endValue }

    MouseArea {
        anchors.fill: parent

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
        target: chunk; property: "endValue";
        to:clickedEndValue;
        duration: 2000;
    }
}
