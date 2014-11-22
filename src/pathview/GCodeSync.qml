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

QtObject {
    property var status: {"synced": false}
    property var model: undefined

    property int _lastLine: 1
    property bool _ready: status.synced

    on_ReadyChanged: {
        if (_ready) {
            status.onMotionChanged.connect(updateLine)
        }
        else {
            status.onMotionChanged.disconnect(updateLine)
        }
    }

    function updateLine() {
        if (_ready) {
            var file = status.task.file
            var currentLine = status.motion.motionLine
            
            if (_lastLine > currentLine) {
                for (var line = 1; line <= _lastLine; ++line) {
                    model.setData(file, line, false, GCodeProgramModel.ExecutedRole)
                    model.setData(file, line, false, GCodeProgramModel.ActiveRole)
                }
                _lastLine = currentLine
            }

            for (line = _lastLine; line < currentLine; ++line) {
                model.setData(file, line, true, GCodeProgramModel.ExecutedRole)
                model.setData(file, line, false, GCodeProgramModel.ActiveRole)
            }

            model.setData(file, currentLine, true, GCodeProgramModel.ActiveRole)

            _lastLine = currentLine
        }
    }
}
