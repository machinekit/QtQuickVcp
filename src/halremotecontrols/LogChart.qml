import QtQuick 2.0
import Machinekit.Controls 1.0 as H
import Machinekit.HalRemote 1.0 as HAL

H.LogChart {
    property string name: "logChart"
    property alias value: logChartPin.value

    id: main

    width: 300
    height: 200
    autoSample: false

    HAL.Pin {
        id: logChartPin

        name: main.name
        type: HAL.Pin.HAL_FLOAT
        direction: HAL.Pin.HAL_IN

        onValueChanged: {
            main.addData(value)
        }
    }
}
