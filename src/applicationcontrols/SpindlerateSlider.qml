import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0
import Machinekit.Application.Controls 1.0

Slider {
    property alias core: handler.core
    property alias status: handler.status
    property alias command: handler.command

    id: root
    minimumValue: handler.minimumValue
    maximumValue: handler.maximumValue
    enabled: handler.enabled

    SpindlerateHandler {
        id: handler
    }

    Binding { target: root; property: "value"; value: handler.value }
    Binding { target: handler; property: "value"; value: root.value }
}
