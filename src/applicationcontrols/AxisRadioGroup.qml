import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

RowLayout {
    property alias core: object.core
    property alias status: object.status
    property int axis: axisGroup.currentIndex
    property var axisNames: ["X", "Y", "Z", "A", "B", "C", "U", "V", "W"]

    id: root

    ExclusiveGroup {
        property int currentIndex: 0
        id: axisGroup
    }

    Repeater {
        model: status.synced ? status.config.axes : 0
        RadioButton {
            exclusiveGroup: axisGroup
            text: root.axisNames[index]
            checked: index == 0
            onCheckedChanged: {
                if (checked) {
                    axisGroup.currentIndex = index
                }
            }
        }
    }

    ApplicationObject {
        id: object
    }
}
