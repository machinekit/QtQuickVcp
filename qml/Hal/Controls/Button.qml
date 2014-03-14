import QtQuick 2.1
import QtQuick.Controls 1.1
import Hal 1.0 as HAL

Button {
    id: main

    property string name

    HAL.Pin {
        id: buttonPin

        name: main.name
        type: HAL.Pin.HAL_BIT
        direction: HAL.Pin.HAL_OUT
    }

    Binding { target: main; property: "pressed"; value: buttonPin.value}
    Binding { target: buttonPin; property: "value"; value: main.pressed}
}
