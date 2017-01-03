/****************************************************************************
**
** Copyright (C) 2016 Yishin Li
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
** Yishin Li @ Arais Robot Technology Inc <ysli AT araisrobo DOT com>
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/

import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

ApplicationAction {
    property bool _ready: status.synced && command.connected && (file.localFilePath !== "")
    property var editor

    id: root
    text: qsTr("Edit")
    //iconName: "mode-edit-white"
    iconSource: "qrc:Machinekit/Application/Controls/icons/mode-edit-white"
    shortcut: "F4"
    tooltip: qsTr("Edit current file [%1]").arg(shortcut)
    checkable: true
    onTriggered: {
        file.editMode = !file.editMode
        editor.visible = file.editMode
    }

    checked: _ready && file.editMode
    enabled: _ready
             && !status.running
}
