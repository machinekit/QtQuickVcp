import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

Slider {
    property var status: {"synced": false}
    property var command: {"connected": false}

    property bool _ready: status.synced && command.connected

    minimumValue: _ready ? status.config.minSpindleOverride : 0
    maximumValue: _ready ? status.config.maxSpindleOverride : 100
    stepSize: 0.01
    enabled: _ready


    onValueChanged: {
        if (_ready && (value !== status.motion.spindlerate)) {
            command.setSpindleOverride(value)
        }
    }

    on_ReadyChanged: {
        if (_ready) {
            _update()
            status.onMotionChanged.connect(_update)
        }
        else {
            status.onMotionChanged.disconnect(_update)
        }
    }

    function _update() {
        value = status.motion.spindlerate
    }
}
