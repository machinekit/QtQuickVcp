import QtQuick 2.0
import QtQuick.Controls 1.1

Item {
    property alias color: mid.color
    property color borderColor: "darkGray"
    property alias backgroundColor: base.color
    property alias foregroundColor: top.color
    property alias textColor: text.color
    property double value: 25
    property alias minimumValue: mid.minimumValue
    property alias maximumValue: mid.maximumValue
    property alias stepSize: mid.stepSize
    property int decimals: 0
    property alias readOnly: mid.readOnly
    property bool pieMultiColor: false
    property bool meter: false
    property string style: "Pie"
    property string needleType: "Point"
    property string pieType: "Flat"
    property string prefix: ""
    property string suffix: ""
    property alias font: text.font
    property real centerScale: needleType == "Groove" ? 0.8 : 0.66

    id: root
    width: 200
    height: 200

    KnobPart {
        id: base
        anchors.fill: parent
        z: 0.1
        color: Qt.rgba(241,241,241,255)
        border.color: root.borderColor
    }

    KnobPie {
        id: mid
        anchors.fill: parent
        z: 0.3
        multicolor: root.pieMultiColor
        style: root.pieType

        MouseArea {
            id: events
            anchors.fill: parent
            enabled: !readOnly

            function calculateValue(x,y) {
                var dx = (x - parent.height/2)
                var dy = (y - parent.width/2)
                value = parent.endValueFromPoint(dx,dy)
            }

            onWheel: {
                wheel.angleDelta.y < 0 ? value < maxValue ? value += stepSize : maxValue : value > minValue ? value -= stepSize : minValue
            }

            onClicked: calculateValue(mouseX,mouseY)
            onPositionChanged: calculateValue(mouseX,mouseY)
        }

        Binding { target: mid; property: "value"; value: root.value; when: !animate.running } // decouple value from animation
        Binding { target: root; property: "value"; value: mid.value; when: !animate.running }

        Behavior on value {
            enabled: !events.pressed
            SmoothedAnimation { id: animate; duration: 800 }
        }
    }

    KnobPart {
        property real w: root.width * root.centerScale

        id: top
        z: 0.4
        x: base.width/2 - w/2
        y: base.height/2 - w/2
        width: w
        height: w
        isBottom: false
        color: "white"
        border.color: root.borderColor
    }

    Label {
        id: text
        anchors.centerIn: parent
        z: 0.5
        text: root.prefix + root.value.toFixed(root.decimals) + root.suffix
    }
}

