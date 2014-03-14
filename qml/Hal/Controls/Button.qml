import QtQuick 2.1
import QtQuick.Controls 1.1
import Hal 1.0 as HAL

Button {
    id: main

    property string name
    checkable: true

    HAL.Pin {
        id: myPin

        name: main.name
        type: HAL.Pin.HAL_BIT
        direction: HAL.Pin.HAL_OUT
        value: main.checked
    }
}
