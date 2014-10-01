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

    property bool _ready: status.synced
    property var _mcodes: _ready ? status.interp.mcodes : []
    property var _gcodes: _ready ? status.interp.gcodes : []

    text: {
        var mcodes = ""
        for (var i = 1; i < _mcodes.length; ++i) {
            if (_mcodes[i] > -1) {
                mcodes += "M" + _mcodes[i].toString() + " "
            }
        }
        var gcodes = ""
        for (i = 1; i < _gcodes.length; ++i) {
            if (_gcodes[i] > -1) {
                gcodes += "G" + (_gcodes[i]/10).toString() + " "
            }
        }
        return gcodes + mcodes
    }

    ApplicationObject {
        id: object
    }
}
