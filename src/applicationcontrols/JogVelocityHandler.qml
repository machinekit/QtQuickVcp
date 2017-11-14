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
    readonly property string units: proportional ? "%" : distanceUnits + "/" + timeUnits
    readonly property string distanceUnits: helper.ready ? helper.distanceUnits : "mm"
    readonly property string timeUnits: helper.ready ? helper.timeUnits : "min"
    readonly property double displayValue: proportional ? value : value * __timeFactor * __distanceFactor
    property double value: 0
    property double minimumValue: 0
    property double maximumValue: 100
    property bool proportional: false
    property double minimumProportion: 0.0
    property bool enabled: __ready
    property bool synced: false

    readonly property double __timeFactor: helper.ready ? helper.timeFactor : 1
    readonly property double __distanceFactor: helper.ready ? helper.distanceFactor : 1
    readonly property bool __ready: status.synced && settings.initialized
    property bool __remoteUpdate: false

    onValueChanged: {
        if (__ready && !__remoteUpdate) {
            var velocity = value;
            if (proportional) {
                velocity /= 100.0;
                velocity *= maximumValue;
            }
            settings.setValue("axis" + axis + ".jogVelocity", velocity);
            synced = false;
        }
    }

    on__ReadyChanged: {
        if (__ready) {
            _update();
            settings.onValuesChanged.connect(_update);
            status.onConfigChanged.connect(_update);
            status.onMotionChanged.connect(_update);
        }
        else {
            settings.onValuesChanged.disconnect(_update);
            status.onConfigChanged.disconnect(_update);
            status.onMotionChanged.disconnect(_update);
            synced = false;
        }
    }

    onAxisChanged: {
        if (__ready) {
            _update();
        }
    }

    Component.onDestruction: {
        if (!settings.onValuesChanged) { // for qmlplugindump
            return;
        }
        settings.onValuesChanged.disconnect(_update);
        status.onConfigChanged.disconnect(_update);
        status.onMotionChanged.disconnect(_update);
    }

    function _update() {
        __remoteUpdate = true;
        minimumValue = status.config.minVelocity;
        var axisMaxVel = status.config.axis[axis].maxVelocity;
        var configMaxVel = status.config.maxVelocity;
        if ((axisMaxVel === undefined) || (axisMaxVel === 0) || (axisMaxVel > configMaxVel)) {
            maximumValue = configMaxVel;
        } else {
            maximumValue = axisMaxVel;
        }
        minimumProportion = (minimumValue / maximumValue) * 100.0;

        var tmpValue = settings.value("axis" + axis + ".jogVelocity");
        tmpValue = Math.max(Math.min(tmpValue, maximumValue), minimumValue); // clamp value
        if (proportional) {
            tmpValue /= maximumValue;
            tmpValue *= 100.0;
        }

        if (value !== tmpValue) {
            value = tmpValue;
        }
        else {
            synced = true;
        }
        __remoteUpdate = false;
    }
}
