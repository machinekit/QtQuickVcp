import QtQuick 2.0
import Hal 1.0 as HAL

Rectangle {
    property bool value: ledPin.value
    property color onColor: "red"
    property color offColor: "darkGrey"
    property bool blink: false
    property int blinkRate: 500
    property string name: "led"

    id: main

    width: 50
    height: 50
    border.width: 2
    radius: width/2
    border.color: "black"
    color: (value && (helpItem.blinkHelper || !blink))?onColor:offColor

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

    HAL.Pin {
        id: ledPin

        name: main.name
        type: HAL.Pin.HAL_BIT
        direction: HAL.Pin.HAL_IN
    }
}
