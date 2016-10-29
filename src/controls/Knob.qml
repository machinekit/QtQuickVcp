import QtQuick 2.0
import QtQuick.Controls 1.1
import Machinekit.Controls.Private 1.0

Item {
    property alias color: mid.color
    property color borderColor: "darkGray"
    property alias backgroundColor: base.color
    property alias foregroundColor: top.color
    property alias textColor: label.color
    property double value: defaultValue
    property alias minimumValue: mid.minimumValue
    property alias maximumValue: mid.maximumValue
    property alias defaultValue: mid.defaultValue
    property alias stepSize: mid.stepSize
    property int decimals: 0
    property alias readOnly: mid.readOnly
    property alias pieMultiColor: mid.multicolor
    property bool meter: false
    property string style: "Pie"
    property string needleType: "Point"
    property alias pieType: mid.style
    property string prefix: ""
    property string suffix: ""
    property alias font: label.font
    property real centerScale: needleType === "Groove" ? 0.8 : 0.66
    property alias text: label.text

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

        Binding { target: mid; property: "value"; value: root.value; when: !mid.animating } // decouple value from animation
        Binding { target: root; property: "value"; value: mid.value; when: !mid.animating }
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
        id: label
        anchors.centerIn: parent
        z: 0.5
        text: root.prefix + root.value.toFixed(root.decimals) + root.suffix
    }
}

