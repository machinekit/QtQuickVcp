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
        minimumValue: root.minimumValue
        maximumValue: root.maximumValue
        stepSize: root.stepSize

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
        id: text
        anchors.centerIn: parent
        z: 0.5
        text: root.prefix + root.value.toFixed(root.decimals) + root.suffix
    }
}

