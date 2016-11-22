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

QtObject {
    property var settings: {"initialized": false}
    property var model: []
    property bool _ready: settings.initialized

    id: mdiHistory

    on_ReadyChanged: {
        if (_ready) {
            _update();
            settings.onValuesChanged.connect(_update);
        }
        else {
            settings.onValuesChanged.disconnect(_update);
        }
    }

    function _update() {
        model = settings.value("mdi.history");
        if (model === null) {
            model = [];
        }
    }

    function add(command) {
        var lastCommand = (model.length > 0) ? model[model.length - 1].command : ""
        if (command !== lastCommand) {
            var tmpModel = JSON.parse(JSON.stringify(model)); // copy array
            tmpModel.push({ "command": command });
            settings.setValue("mdi.history", tmpModel);
        }
    }

    function remove(index) {
        var tmpModel = JSON.parse(JSON.stringify(model)); // copy array
        tmpModel.splice(index, 1);
        settings.setValue("mdi.history", tmpModel);
    }

    function insert(index, command) {
        var tmpModel = JSON.parse(JSON.stringify(model)); // copy array
        tmpModel.splice(index, 0, command);
        settings.setValue("mdi.history", tmpModel);
    }

    function clear() {
        settings.setValue("mdi.history", [])
    }
}
