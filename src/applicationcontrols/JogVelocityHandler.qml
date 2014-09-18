import QtQuick 2.0
import Machinekit.Application 1.0

QtObject {
    property var status: {"synced": false}
    property var settings: {"values": undefined}
    property int axis: 0
    readonly property string units: distanceUnits + "/" + timeUnits
    readonly property string distanceUnits: getDistanceUnits()
    readonly property string timeUnits: getTimeUnits()
    readonly property double displayValue: value * _timeFactor
    property double value: 0
    property double minimumValue: _ready ? status.config.minVelocity : 0
    property double maximumValue: _ready ? status.motion.maxVelocity : 100
    property bool enabled: _ready

    property double _timeFactor: (timeUnits == "min") ? 60 : 1
    property bool _ready: status.synced && settings.initialized

    onValueChanged: {
        if (_ready && (value !== status.config.velocity)) {
            settings.setValue("axis" + axis + ".jogVelocity", value)
        }
    }

    on_ReadyChanged: {
        if (_ready) {
            _update()
            settings.onValuesChanged.connect(_update)
        }
        else {
            settings.onValuesChanged.disconnect(_update)
        }
    }

    onAxisChanged: {
        if (_ready) {
            _update()
        }
    }

    function _update() {
        value = settings.value("axis" + axis + ".jogVelocity")
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
