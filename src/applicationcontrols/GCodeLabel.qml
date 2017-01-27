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
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

Label {
    property alias core: object.core
    property alias status: object.status

    property var desiredMCodes: [1,2,3,4,5,6,7,8,9]
    property var desiredGCodes: [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]

    property bool _ready: status.synced
    property var _mcodes: _ready ? status.interp.mcodes : []
    property var _gcodes: _ready ? status.interp.gcodes : []

    text: {
        var mcodes = [];
        var gcodes = [];
        var desired = [];
        for (var i = 0; i < desiredMCodes.length; ++i) {
            desired = desiredMCodes[i];
            if (_mcodes[desired] > -1) {
                mcodes.push("M" + _mcodes[desired].toString());
            }
        }
        for (i = 0; i < desiredGCodes.length; ++i) {
            desired = desiredGCodes[i];
            if (_gcodes[desired] > -1) {
                gcodes.push("G" + (_gcodes[desired]/10).toString());
            }
        }
        return gcodes.join(" ") + " " + mcodes.join(" ");
    }

    ApplicationObject {
        id: object
    }
}
