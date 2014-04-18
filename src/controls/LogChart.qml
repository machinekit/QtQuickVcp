import QtQuick 2.0

ValueChart {
    property alias targetValue: valueModel.targetValue
    property alias maxLogSize: valueModel.maxSize
    property alias sampleInterval: sampleTimer.interval
    property alias updateInterval: updateTimer.interval
    property alias autoUpdate: updateTimer.running
    property alias autoSample: sampleTimer.running
    property double value: 0.0

    id: chart

    width: 100
    height: 62
    valueModel: valueModel

    ValueModel {
        id: valueModel
    }

    Timer {
        id: sampleTimer

        interval: 10
        running: true
        repeat: true
        onTriggered: {
            valueModel.addData(value)
        }
    }

    Timer {
        id: updateTimer

        interval: 100
        running: true
        repeat: true
        onTriggered: {
            chart.update()
        }
    }

    function addData(value)
    {
        valueModel.addData(value)
    }

    function clearData()
    {
        valueModel.clearData()
    }
}
