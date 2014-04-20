import QtQuick 2.0
import Machinekit.Controls 1.0
import Machinekit.HalRemote 1.0

ColorPicker {
    property string name: "colorPicker"

    id: main
    width: 400
    height: 300

    Pin {
        id: rPin
        name: main.name + ".r"
        type: Pin.HAL_U32
        direction: Pin.HAL_OUT
    }
    Pin {
        id: gPin
        name: main.name + ".g"
        type: Pin.HAL_U32
        direction: Pin.HAL_OUT
    }
    Pin {
        id: bPin
        name: main.name + ".b"
        type: Pin.HAL_U32
        direction: Pin.HAL_OUT
    }
    Pin {
        id: aPin
        name: main.name + ".a"
        type: Pin.HAL_U32
        direction: Pin.HAL_OUT
    }
    Pin {
        id: hPin
        name: main.name + ".h"
        type: Pin.HAL_FLOAT
        direction: Pin.HAL_OUT
    }
    Pin {
        id: sPin
        name: main.name + ".s"
        type: Pin.HAL_FLOAT
        direction: Pin.HAL_OUT
    }
    Pin {
        id: vPin
        name: main.name + ".v"
        type: Pin.HAL_FLOAT
        direction: Pin.HAL_OUT
    }

    Binding { target: rPin; property: "value"; value: parseInt(main.colorValue.toString().substr(1, 2), 16)}
    Binding { target: gPin; property: "value"; value: parseInt(main.colorValue.toString().substr(3, 2), 16)}
    Binding { target: bPin; property: "value"; value: parseInt(main.colorValue.toString().substr(5, 2), 16)}
    Binding { target: aPin; property: "value"; value: Math.ceil(main.colorValue.a * 255)}
    Binding { target: hPin; property: "value"; value: main.colorValue.h}    //TODO
    Binding { target: sPin; property: "value"; value: main.colorValue.s}
    Binding { target: vPin; property: "value"; value: main.colorValue.v}
}
