/****************************************************************************
**
** Copyright (C) 2015 Alexander Rössler
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

MdiCommandAction {
    property var commands: {
        var commandList = [];
        if ((status.synced) && (status.config.userCommand !== undefined))
        {
            for (var i = 0; i < status.config.userCommand.length; ++i)
            {
                var command = status.config.userCommand[i];
                if (command === "") {
                    continue;
                }
                var splitted = command.split(" ");
                var mdiCommands = splitted[0].split(";");
                splitted.splice(0,1);
                var name = splitted.join(" ");
                commandList.push({ "name": name, "commands": mdiCommands });
            }
        }
        return commandList;
    }

    function executeCommand(index) {
        if ((index < 0 ) || (index >= commands.length)) {
            return;
        }

        var commandList = commands[index].commands;
        for (var i = 0; i < commandList.length;  ++i)
        {
            root.mdiCommand = commandList[i];
            root.trigger();
        }
    }

    id: root
    enableHistory: false
}
