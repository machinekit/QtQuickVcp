/****************************************************************************
**
** Copyright (C) 2017 Alexander Rössler
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
** Alexander Rössler <alexander AT roessler DOT systems>
**
****************************************************************************/

import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

ApplicationAction {
    property bool _localFileValid: file.localFilePath !== "file://"
    property bool _fileReady: file.transferState === ApplicationFile.NoTransfer
    property bool _ready: status.synced && file.ready && _localFileValid && _fileReady

    id: root
    text: qsTr("&Edit File with System Editor...")
    iconSource: "qrc:Machinekit/Application/Controls/icons/document-edit"
    shortcut: "Ctrl+E"
    tooltip: qsTr("Edit G-Code file with System Editor [%1]").arg(shortcut)
    onTriggered: {
        if (_ready)
        {
            ApplicationHelpers.openUrlWithDefaultApplication(file.localFilePath);
        }
    }
    enabled: _ready && !status.running
}
