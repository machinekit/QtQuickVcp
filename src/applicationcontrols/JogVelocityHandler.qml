/****************************************************************************
**
** Copyright (C) 2014 Alexander Rössler
** License: LGPL version 2.1
**
** This file is part of QtQuickVcp.
**
** All rights reserved. This program and the accompanying materials
** are made available under the terms of the GNU Lesser General Public License
** (LGPL) version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** Contributors:
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/

import QtQuick 2.0
import Machinekit.Application 1.0

ApplicationObject {
    property int axis: 0
    readonly property string units: distanceUnits + "/" + timeUnits
    readonly property string distanceUnits: getDistanceUnits()
    readonly property string timeUnits: getTimeUnits()
    readonly property double displayValue: value * _timeFactor
    property double value: 0
    property double minimumValue: 0
    property double maximumValue: 100
    property bool enabled: _ready
    property bool synced: false

    property double _timeFactor: (timeUnits == "min") ? 60 : 1
    property bool _ready: status.synced && settings.initialized
    property bool _remoteUpdate: false

    onValueChanged: {
        if (_ready && !_remoteUpdate) {
            settings.setValue("axis" + axis + ".jogVelocity", value)
            synced = false
        }
    }

    on_ReadyChanged: {
        if (_ready) {
            _update()
            settings.onValuesChanged.connect(_update)
            status.onConfigChanged.connect(_update)
            status.onMotionChanged.connect(_update)
        }
        else {
            settings.onValuesChanged.disconnect(_update)
            status.onConfigChanged.disconnect(_update)
            status.onMotionChanged.disconnect(_update)
            synced = false
        }
    }

    onAxisChanged: {
        if (_ready) {
            _update()
        }
    }

    Component.onDestruction: {
        settings.onValuesChanged.disconnect(_update)
        status.onConfigChanged.disconnect(_update)
        status.onMotionChanged.disconnect(_update)
    }

    function _update() {
        _remoteUpdate = true
        minimumValue = status.config.minVelocity
        var axisMaxVel = status.config.axis[axis].maxVelocity
        var configMaxVel = status.config.maxVelocity
        if ((axisMaxVel === undefined) || (axisMaxVel === 0) || (axisMaxVel > configMaxVel)) {
            maximumValue = configMaxVel
        } else {
            maximumValue = axisMaxVel
        }
        var tmpValue = settings.value("axis" + axis + ".jogVelocity")
        tmpValue = Math.max(Math.min(tmpValue, maximumValue), minimumValue) // clamp value
        if (value !== tmpValue) {
            value = tmpValue
        }
        else {
            synced = true
        }
        _remoteUpdate = false
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
