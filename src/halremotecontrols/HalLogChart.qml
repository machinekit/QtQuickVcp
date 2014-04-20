import QtQuick 2.0
import Machinekit.Controls 1.0
import Machinekit.HalRemote 1.0

LogChart {
    property string name: "logChart"
    property alias value: logChartPin.value

    id: main

    width: 300
    height: 200
    autoSample: false

    Pin {
        id: logChartPin

        name: main.name
        type: Pin.HAL_FLOAT
        direction: Pin.HAL_IN

        onValueChanged: {
            main.addData(value)
        }
    }
}
