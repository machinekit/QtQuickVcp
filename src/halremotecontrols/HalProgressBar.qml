import QtQuick 2.0
import QtQuick.Controls 1.1
import Machinekit.HalRemote 1.0

ProgressBar {
    property string name: "progressBar"
    property bool showText: true
    property int digits: 2

    id: main

    maximumValue: pinScale.value    // can be overwritten

    Pin {
        id: pin

        name: main.name
        type: Pin.HAL_FLOAT
        direction: Pin.HAL_IN
    }

    Pin {
        id: pinScale

        name: main.name + ".scale"
        type: Pin.HAL_FLOAT
        direction: Pin.HAL_IN
    }

    Text {
        id: progressText

        anchors.centerIn: parent
        text: main.value.toFixed(digits)
        visible: main.showText
    }

    Binding { target: main; property: "value"; value: pin.value}
}
