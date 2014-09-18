import QtQuick 2.0
import Machinekit.Application 1.0

QtObject {
    property var status: {"synced": false}
    property var command: {"connected": false}
    property double value : 0
    property double minimumValue: _ready ? status.config.minFeedOverride : 0
    property double maximumValue: _ready ? status.config.maxFeedOverride : 100
    property bool enabled: _ready

    property bool _ready: status.synced && command.connected

    onValueChanged: {
        if (_ready && (value !== status.motion.feedrate)) {
            command.setFeedOverride(value)
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
        value = status.motion.feedrate
    }
}
