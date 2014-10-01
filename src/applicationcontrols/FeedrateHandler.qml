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
