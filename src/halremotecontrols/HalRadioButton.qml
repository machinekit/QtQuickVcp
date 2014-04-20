import QtQuick 2.1
import QtQuick.Controls 1.1
import Machinekit.HalRemote 1.0

RadioButton {
    property string name: "radio"
    property alias pinDirection: radioPin.direction

    id: main

    checked: radioPin.value
    enabled: false

    Pin {
        id: radioPin

        name: main.name
        type: Pin.HAL_BIT
        direction: Pin.HAL_IN
    }
}
