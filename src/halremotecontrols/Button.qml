import QtQuick 2.1
import QtQuick.Controls 1.1
import Machinekit.HalRemote 1.0 as HAL

Button {
    property string name: "button"
    property alias pinDirection: pin.direction

    id: main

    HAL.Pin {
        id: pin

        name: main.name
        type: HAL.Pin.HAL_BIT
        direction: HAL.Pin.HAL_OUT
    }

    Binding { target: main; property: "pressed"; value: pin.value}
    Binding { target: pin; property: "value"; value: main.pressed}
}
