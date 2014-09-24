import QtQuick 2.0
import Machinekit.Application 1.0

ApplicationObject {
    readonly property string units: distanceUnits + "/" + timeUnits
    readonly property string distanceUnits: getDistanceUnits()
    readonly property string timeUnits: getTimeUnits()
    readonly property double displayValue: value * _timeFactor
    property double minimumValue: 0.01
    property double maximumValue: 5
    property double value: 0
    property bool enabled: _ready
    property bool synced: false

    property double _timeFactor: (timeUnits == "min") ? 60 : 1
    property bool _ready: status.synced && command.connected
    property bool _remoteUpdate: false

    onValueChanged: {
        if (_ready && !_remoteUpdate) {
            command.setMaximumVelocity(value)
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
        _remoteUpdate = true                    // set remote
        minimumValue = status.config.minVelocity
        maximumValue = status.config.maxVelocity
        if (value !== status.motion.maxVelocity) {
            value = status.motion.maxVelocity   // triggers update
        }
        else
        {
            synced = true
        }
        _remoteUpdate = false                   // clear remote
    }

    function getTimeUnits() {
        if (_ready) {
            switch (status.config.timeUnits) {
            case ApplicationStatus.TimeUnitsMinute:
                return "min"
            case ApplicationStatus.TimeUnitsSecond:
                return "s"
            }
        }

        return "min"
    }

    function getDistanceUnits() {
        if (_ready) {
            switch (status.config.programUnits) {
            case ApplicationStatus.CanonUnitsInches:
                return "in"
            case ApplicationStatus.CanonUnitsMm:
                return "mm"
            case ApplicationStatus.CanonUnitsCm:
                return "cm"
            }
        }

        return "mm"
    }
}
