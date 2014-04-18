import QtQuick 2.1
import Machinekit.HalRemote 1.0 as HAL
import Machinekit.Controls 1.0 as H

H.VirtualJoystick {
    property string name: "joystick"
    property alias pinDirection: xVelocityPin.direction

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
        type: xVelocityPin.type
        direction: xVelocityPin.direction
        value: main.yVelocity
    }
}
