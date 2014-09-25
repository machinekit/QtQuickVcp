import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

ComboBox {
    property alias core: object.core
    property alias status: object.status
    property int axis: currentIndex
    property var axisNames: ["X", "Y", "Z", "A", "B", "C", "U", "V", "W"]

    model: {
        var axes = status.synced ? status.config.axes : 0
        var model = []
        for (var i = 0; i < axes; ++i) {
            model.push(axisNames[i])
        }
        return model
    }

    ApplicationObject {
        id: object
    }
}
