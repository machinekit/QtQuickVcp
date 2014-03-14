import QtQuick 2.1
import Hal 1.0 as HAL
import FancyControls 1.0

VirtualJoystick {
    property string name: "joystick"

    id: main

    width: 300
    height: 400

    HAL.Pin {
        id: xVelocityPin

        name: main.name + ".x"
        type: HAL.Pin.HAL_FLOAT
        direction: HAL.Pin.HAL_OUT
        value: main.xVelocity
    }

    HAL.Pin {
        id: yVelocityPin

        name: main.name + ".y"
        type: HAL.Pin.HAL_FLOAT
        direction: HAL.Pin.HAL_OUT
        value: main.yVelocity
    }
}
