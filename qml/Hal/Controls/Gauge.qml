import QtQuick 2.0
import QtQuick.Controls 1.1
import Hal 1.0 as HAL

Item {
    property string name: "progressBar"
    property double minimumValue: -100
    property alias maximumValue: progressBar2.maximumValue
    property bool showText: true
    property double value: 0

     id: main

    width: 100
    height: 62

    ProgressBar {
        //maximumValue: pinScale.value    // can be overwritten
        id: progressBar1

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width * 0.5

        minimumValue: 0
        maximumValue: -main.minimumValue
        rotation: 180

        value: (main.value < 0)?-main.value:0
    }

    ProgressBar {
        //maximumValue: pinScale.value    // can be overwritten
        id: progressBar2

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: progressBar1.right
        anchors.right: parent.right

        minimumValue: 0
        maximumValue: 100

        value: (main.value > 0)?main.value:0
    }

    Text {
        id: progressText1

        anchors.centerIn: parent
        text: main.value.toFixed(2)
        visible: main.showText
    }

    HAL.Pin {
        id: pin

        name: main.name
        type: HAL.Pin.HAL_FLOAT
        direction: HAL.Pin.HAL_IN
    }

    HAL.Pin {
        id: pinScale

        name: main.name + ".scale"
        type: HAL.Pin.HAL_FLOAT
        direction: HAL.Pin.HAL_IN
    }

    Binding { target: main; property: "value"; value: pin.value}
}
