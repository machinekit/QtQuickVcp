import QtQuick 2.1
import QtQuick.Controls 1.1
import Machinekit.HalRemote 1.0

Button {
    property string name: "toggleButton"
    property alias pinDirection: pin.direction

    id: main

    checkable: true

    Pin {
        id: pin

        name: main.name
        type: Pin.HAL_BIT
        direction: Pin.HAL_OUT
    }

    Binding { target: main; property: "checked"; value: pin.value}
    Binding { target: pin; property: "value"; value: main.checked}
}
