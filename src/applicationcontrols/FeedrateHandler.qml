import QtQuick 2.0
import Machinekit.Application 1.0

ApplicationObject {
    property double value : 0
    property double minimumValue: 0
    property double maximumValue: 100
    property bool enabled: _ready
    property bool synced: false

    property bool _ready: status.synced && command.connected
    property bool _remoteUpdate: false

    onValueChanged: {
        if (_ready && !_remoteUpdate) {
            command.setFeedOverride(value)
            synced = false
        }
    }

    on_ReadyChanged: {
        if (_ready) {
            _update()
            status.onMotionChanged.connect(_update)
            status.onConfigChanged.connect(_update)
        }
        else {
            status.onMotionChanged.disconnect(_update)
            status.onConfigChanged.disconnect(_update)
            synced = false
        }
    }

    function _update() {
        _remoteUpdate = true
        minimumValue = status.config.minFeedOverride
        maximumValue = status.config.maxFeedOverride
        if (value !== status.motion.feedrate) {
            value = status.motion.feedrate
        }
        else {
            synced = true
        }
        _remoteUpdate = false
    }
}
