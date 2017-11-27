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
import Machinekit.PathView 1.0

Item {
    property var status: { "synced": false }
    property var model: undefined

    QtObject {
        id: d
        readonly property int currentLine: status.synced ? status.motion.motionLine: 1
        readonly property bool taskIsInReliableState: status.synced ? (status.task.execState === ApplicationStatus.TaskWaitingForMotion
                                                                       || status.task.execState === ApplicationStatus.TaskDone) : false
        readonly property string file: status.synced ? status.task.file : ""
        property int lastLine: 1
        readonly property bool ready: status.synced

        onCurrentLineChanged: updateLine()
        onFileChanged: updateLine()

        function updateLine() {
            if (!d.ready || !taskIsInReliableState) {
                return;
            }

            if (d.lastLine > d.currentLine) {
                for (var line = 1; line <= d.lastLine; ++line) {
                    model.setData(file, line, false, GCodeProgramModel.ExecutedRole);
                    model.setData(file, line, false, GCodeProgramModel.ActiveRole);
                }
                d.lastLine = d.currentLine;
            }

            for (line = d.lastLine; line < d.currentLine; ++line) {
                model.setData(file, line, true, GCodeProgramModel.ExecutedRole);
                model.setData(file, line, false, GCodeProgramModel.ActiveRole);
            }

            model.setData(file, currentLine, true, GCodeProgramModel.ActiveRole);

            d.lastLine = d.currentLine;
        }
    }
}
