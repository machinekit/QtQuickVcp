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
    property int programStartLine: 0
    property int programResetLine: 0

    property bool _ready: status.synced && command.connected
    property bool _fileValid: status.synced
                              && (status.task.file !== "")
                              && (status.task.file.indexOf(status.config.remotePath) === 0)

    id: root
    text: qsTr("Run")
    iconSource: "qrc:Machinekit/Application/Controls/icons/go-next-view"
    shortcut: "R"
    tooltip: qsTr("Begin executing current file [%1]").arg(shortcut)
    onTriggered: {
        if (status.task.taskMode !== ApplicationStatus.TaskModeAuto) {
            command.setTaskMode('execute', ApplicationCommand.TaskModeAuto);
        }
        command.runProgram('execute', programStartLine);
        programResetLine = programStartLine;
    }
    enabled: _ready
             && _fileValid
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && !status.running
             && (status.motion.state !== ApplicationStatus.MotionExec)
}
