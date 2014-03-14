import QtQuick 2.1
import QtQuick.Controls 1.1
import Hal 1.0 as HAL

Button {
    property string name: "toggleButton"

    id: main

    checkable: true

    HAL.Pin {
        id: pin

        name: main.name
        type: HAL.Pin.HAL_BIT
        direction: HAL.Pin.HAL_OUT
    }

    Binding { target: main; property: "checked"; value: pin.value}
    Binding { target: pin; property: "value"; value: main.checked}
}
