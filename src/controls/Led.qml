import QtQuick 2.0

Rectangle {
    property string name: "led"
    property bool value: false
    property color onColor: "red"
    property color offColor: "darkGrey"
    property bool blink: false
    property int blinkRate: 500
    property bool shine: true

    id: main

    width: 50
    height: 50
    border.width: 2
    radius: width/2
    border.color: "black"
    color: (value && (helpItem.blinkHelper || !blink))?onColor:offColor

    Rectangle {
        x: parent.width*0.15
        y: parent.width*0.15
        width: parent.width*0.4
        height: width
        radius: width/2
        color: "white"
        opacity: 0.4
        visible: main.shine
    }

    Timer {
        id: blinkTimer
        running: blink
        repeat: true
        interval: blinkRate
        onTriggered: helpItem.blinkHelper = !helpItem.blinkHelper
    }

    Item {
        id: helpItem
        property bool blinkHelper: true
    }
}
