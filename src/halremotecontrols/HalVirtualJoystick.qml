import QtQuick 2.1
import Machinekit.HalRemote 1.0
import Machinekit.Controls 1.0

VirtualJoystick {
    property string name: "joystick"
    property alias pinDirection: xVelocityPin.direction

    id: main

    width: 300
    height: 400

    Pin {
        id: xVelocityPin

        name: main.name + ".x"
        type: Pin.HAL_FLOAT
        direction: Pin.HAL_OUT
        value: main.xVelocity
    }

    Pin {
        id: yVelocityPin

        name: main.name + ".y"
        type: xVelocityPin.type
        direction: xVelocityPin.direction
        value: main.yVelocity
    }
}
