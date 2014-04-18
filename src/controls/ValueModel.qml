import QtQuick 2.0

ListModel {
    id: model
    property string name: ""
    property real startTimestamp
    property real endTimestamp
    property double highestValue: 0.0
    property double lowestValue: 0.0
    property double currentValue: 0.0
    property double targetValue: 0.0
    property bool ready: false
    property int maxSize: 5000

    signal dataReady

    function indexOf(timestamp) {
        var start = model.get(0).timestamp;
        var end = model.get(model.count - 1).timestamp;
        if (end <= timestamp)
            return model.count -1;

        if (start >= timestamp)
            return 0;

        for (var i = 0; i < model.count; i++) {
            if (model.get(i).timestamp > timestamp)
                return i-1;
        }
        return -1;
    }

    function createValue(value) {
        if (highestValue < value)
            highestValue = value;
        if (lowestValue > value)
            lowestValue = value;
        if (model.count == 0)
            startTimestamp = Date.now()
        endTimestamp = Date.now()
        currentValue = value
        return {
                "timestamp": endTimestamp,
                "value":value
               };
    }

    function clearData() {
        model.clear()
        model.ready = false
    }

    function addData(value)
    {
        model.append(createValue(value))
        if (model.count > maxSize)
            model.remove(0)
        model.ready = true
        model.dataReady(); //emit signal
    }
}
