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

Action {
    property var core: null
    property var status: core === null ? {"synced": false} : core.status
    property var settings: core === null ? {"initialized": false} : core.settings
    property var command: core === null ? {"connected": false} : core.command
    property var file: core === null ? {"localPath":"", "remotePath":"", "localFilePath":"", "ready":false} : core.file
    property var error: core === null ? {"connected": false} : core.error
    property var mdiHistory: core == null ? {"model": []} : core.mdiHistory
    property var homeAllAxesHelper: core == null ? {"running": false} : core.homeAllAxesHelper

    Component.onCompleted: {
        if (core == null)
        {
            try {
                var x = applicationCore
                core = Qt.binding(function() {return x})
            }
            catch (err) {
            }
        }
    }
}
