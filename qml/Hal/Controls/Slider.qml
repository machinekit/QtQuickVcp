import QtQuick 2.1
import QtQuick.Controls 1.1
import Hal 1.0 as HAL

Slider {
    property string name: "slider"

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

    Binding { target: main; property: "value"; value: sliderPin.value}
    Binding { target: sliderPin; property: "value"; value: main.value}
}
