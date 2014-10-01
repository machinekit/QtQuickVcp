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
import Machinekit.Controls 1.0
import Machinekit.Application 1.0

VirtualJoystick {
    property alias core: object.core
    property alias status: object.status
    property alias command: object.command
    property int xAxis: 0
    property int yAxis: 1

    property bool _ready: status.synced && command.connected

    id: root

    function _jog(axis, value) {
        if (axis === -1)
            return

        if (status.task.taskMode !== ApplicationStatus.TaskModeManual)
            command.setTaskMode('execute', ApplicationCommand.TaskModeManual)
        if (value !== 0.0) {
            command.jog(ApplicationCommand.ContinousJog, axis, value)
        } else {
            command.jog(ApplicationCommand.StopJog, axis)
        }
    }

    enabled: _ready
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && !status.running

    onXValueChanged: _jog(xAxis, xValue)

    onYValueChanged: _jog(yAxis, yValue)

    ApplicationObject {
        id: object
    }
}
