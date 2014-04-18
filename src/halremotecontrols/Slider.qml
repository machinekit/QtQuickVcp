import QtQuick 2.1
import QtQuick.Controls 1.1
import Machinekit.HalRemote 1.0 as HAL

Slider {
    property string name: "slider"
    property bool showText: true
    property alias pinDirection: sliderPin.direction

    id: main

    width: 100
    height: 62
    //value: velocityOut.value

    HAL.Pin {
        id: sliderPin

        name: main.name
        type: HAL.Pin.HAL_FLOAT
        direction: HAL.Pin.HAL_OUT
    }

    Text {
        id: valueText

        x: parent.width * 0.99 * (main.value/main.maximumValue) - width/2
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: - parent.height * 0.3
        width: parent.width*0.2
        horizontalAlignment: Text.AlignHCenter
        text: main.value.toFixed(2)
        visible: main.showText
    }

    Binding { target: main; property: "value"; value: sliderPin.value}
    Binding { target: sliderPin; property: "value"; value: main.value}
}
