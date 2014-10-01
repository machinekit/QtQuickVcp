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
    property string pauseShortcut: "P"
    property string resumeShortcut: "S"

    property bool _ready: status.synced && command.connected

    id: root
    checkable: true
    text: checked ? qsTr("Resume") : qsTr("Pause")
    iconSource: "qrc:Machinekit/Application/Controls/icons/go-pause"
    shortcut: checked ? resumeShortcut : pauseShortcut
    tooltip: (checked ? qsTr("Pause execution") : qsTr("Resume execution")) + " [" + shortcut + "]"
    onTriggered: {
        if (status.task.taskMode !== ApplicationStatus.TaskModeAuto)
            command.setTaskMode('execute', ApplicationCommand.TaskModeAuto)
        if (checked) {
            command.pauseProgram('execute');
        } else {
            command.resumeProgram('execute');
        }
    }
    enabled: _ready
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && status.running
}
