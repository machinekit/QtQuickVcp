import QtQuick 2.1
import QtQuick.Controls 1.1
import Hal 1.0 as HAL

RadioButton {
    property string name: "radio"

    id: main

    checked: radioPin.value
    enabled: false

    HAL.Pin {
        id: radioPin

        name: main.name
        type: HAL.Pin.HAL_BIT
        direction: HAL.Pin.HAL_IN
    }
}
