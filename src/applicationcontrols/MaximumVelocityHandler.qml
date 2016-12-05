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
    readonly property string units: proportional ? "%" : distanceUnits + "/" + timeUnits
    readonly property string distanceUnits: helper.ready ? helper.distanceUnits : "mm"
    readonly property string timeUnits: helper.ready ? helper.timeUnits : "min"
    readonly property double displayValue: proportional ? value : value * _timeFactor * _distanceFactor
    property double minimumValue: 0.0
    property double maximumValue: 5.0
    property bool proportional: false
    property double minimumProportion: 0.0
    property double value: 0.0
    property bool enabled: _ready
    property bool synced: false

    property double _timeFactor: helper.ready ? helper.timeFactor : 1
    property double _distanceFactor: helper.ready ? helper.distanceFactor : 1
    property bool _ready: status.synced && command.connected
    property bool _remoteUpdate: false

    onValueChanged: {
        if (_ready && !_remoteUpdate) {
            var maxVelocity = value;
            if (proportional) {
                maxVelocity /= 100.0;
                maxVelocity *= maximumValue;
            }
            command.setMaximumVelocity(maxVelocity);
            synced = false;
        }
    }

    on_ReadyChanged: {
        if (_ready) {
            _update()
            status.onMotionChanged.connect(_update);
            status.onConfigChanged.connect(_update);
        }
        else {
            status.onMotionChanged.disconnect(_update);
            status.onConfigChanged.disconnect(_update);
            synced = false;
        }
    }

    function _update() {
        _remoteUpdate = true;                    // set remote
        minimumValue = status.config.minVelocity;
        maximumValue = status.config.maxVelocity;
        minimumProportion = (minimumValue / maximumValue) * 100.0;

        var maxVelocity = status.motion.maxVelocity;
        maxVelocity = Math.max(Math.min(maxVelocity, maximumValue), minimumValue);
        if (proportional) {
            maxVelocity /= maximumValue;
            maxVelocity *= 100.0;
        }

        if (value !== maxVelocity) {
            value = maxVelocity;   // triggers update
        }
        else
        {
            synced = true;
        }
        _remoteUpdate = false;                   // clear remote
    }
}
