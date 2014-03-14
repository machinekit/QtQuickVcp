import QtQuick 2.0
import QtQuick.Controls 1.1
import Hal 1.0 as HAL

ProgressBar {
    property string name: "progressBar"

    id: main

    HAL.Pin {
        id: pin

        name: main.name
        type: HAL.Pin.HAL_FLOAT
        direction: HAL.Pin.HAL_IN
    }

    Binding { target: main; property: "value"; value: pin.value}
}
