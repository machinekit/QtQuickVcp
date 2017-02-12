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

ApplicationAction {
    property int axis: 0
    readonly property bool homed: _ready ? (!_homeAll ? status.motion.axis[axis].homed : homeAllAxesHelper.allHomed) : false

    readonly property bool _ready: status.synced && command.connected
    readonly property bool _homeAll: root.axis < 0

    id: root
    text: (axis > -1) ? qsTr("Home") : qsTr("Home All")
    shortcut: "Ctrl+Home"
    tooltip: (!_homeAll ? (qsTr("Home axis %1 [%2]").arg(axis)) : qsTr("Home all axes [%1]")).arg(shortcut)
    enabled: _ready
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && !status.running
             && !(_homeAll && !homeAllAxesHelper.homingOrderDefined)
    onTriggered: _triggerHoming()

    function _triggerHoming() {
        if (status.task.taskMode !== ApplicationStatus.TaskModeManual) {
            command.setTaskMode('execute', ApplicationCommand.TaskModeManual);
        }

        if (!_homeAll) {
            command.homeAxis(axis);
        }
        else {
            homeAllAxesHelper.trigger();
        }
    }
}

