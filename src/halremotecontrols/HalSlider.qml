import QtQuick 2.1
import QtQuick.Controls 1.1
import Machinekit.HalRemote 1.0

Slider {
    property string name: "slider"
    property bool showText: true
    property alias pinDirection: sliderPin.direction
    property int digits: 2

    id: main

    width: 200
    height: 62

    Pin {
        id: sliderPin

        name: main.name
        type: Pin.HAL_FLOAT
        direction: Pin.HAL_OUT
    }

    Text {
        id: valueText

        x: (parent.width - width) * (main.value/main.maximumValue)
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: - parent.height * 0.3
        horizontalAlignment: Text.AlignLeft
        text: main.value.toFixed(digits)
        visible: main.showText
    }

    Binding { target: main; property: "value"; value: sliderPin.value}
    Binding { target: sliderPin; property: "value"; value: main.value}
}
