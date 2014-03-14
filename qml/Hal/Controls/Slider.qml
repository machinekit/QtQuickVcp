import QtQuick 2.1
import QtQuick.Controls 1.1
import Hal 1.0 as HAL

Slider {
    property string name: "slider"

    id: main

    width: 100
    height: 62
    value: velocityOut.value

    HAL.Pin {
        id: velocityOut

        name: main.name
        type: HAL.Pin.HAL_FLOAT
        direction: HAL.Pin.HAL_IN
    }

    //Binding { target: virtualJoyStick; property: "xVelocity"; value: test.value}
    //Binding { target: test; property: "value"; value: virtualJoyStick.xVelocity}
}
