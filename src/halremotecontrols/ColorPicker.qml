import QtQuick 2.0
import Machinekit.Controls 1.0
import Machinekit.HalRemote 1.0 as HAL

ColorPicker {
    property string name: "colorPicker"

    id: main
    width: 400
    height: 300

    HAL.Pin {
        id: rPin
        name: main.name + ".r"
        type: HAL.Pin.HAL_U32
        direction: HAL.Pin.HAL_OUT
    }
    HAL.Pin {
        id: gPin
        name: main.name + ".g"
        type: HAL.Pin.HAL_U32
        direction: HAL.Pin.HAL_OUT
    }
    HAL.Pin {
        id: bPin
        name: main.name + ".b"
        type: HAL.Pin.HAL_U32
        direction: HAL.Pin.HAL_OUT
    }
    HAL.Pin {
        id: aPin
        name: main.name + ".a"
        type: HAL.Pin.HAL_U32
        direction: HAL.Pin.HAL_OUT
    }
    HAL.Pin {
        id: hPin
        name: main.name + ".h"
        type: HAL.Pin.HAL_FLOAT
        direction: HAL.Pin.HAL_OUT
    }
    HAL.Pin {
        id: sPin
        name: main.name + ".s"
        type: HAL.Pin.HAL_FLOAT
        direction: HAL.Pin.HAL_OUT
    }
    HAL.Pin {
        id: vPin
        name: main.name + ".v"
        type: HAL.Pin.HAL_FLOAT
        direction: HAL.Pin.HAL_OUT
    }

    Binding { target: rPin; property: "value"; value: parseInt(main.colorValue.toString().substr(1, 2), 16)}
    Binding { target: gPin; property: "value"; value: parseInt(main.colorValue.toString().substr(3, 2), 16)}
    Binding { target: bPin; property: "value"; value: parseInt(main.colorValue.toString().substr(5, 2), 16)}
    Binding { target: aPin; property: "value"; value: Math.ceil(main.colorValue.a * 255)}
    Binding { target: hPin; property: "value"; value: main.colorValue.h}    //TODO
    Binding { target: sPin; property: "value"; value: main.colorValue.s}
    Binding { target: vPin; property: "value"; value: main.colorValue.v}
}
