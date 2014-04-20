import QtQuick 2.0
import Machinekit.HalRemote 1.0
import Machinekit.Controls 1.0

Led {
    id: main

    width: 50
    height: 50

    Pin {
        id: ledPin

        name: main.name
        type: Pin.HAL_BIT
        direction: Pin.HAL_IN
    }

    Binding { target: main; property: "value"; value: ledPin.value}
}
