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
import Machinekit.Service 1.0
import Machinekit.PathView 1.0
import Machinekit.Application 1.0

ApplicationItem {
    property alias gcodeProgramModel: gcodeProgramModel
    property alias gcodeProgramLoader: gcodeProgramLoader

    property bool _ready: file.ready
    property bool _previewEnabled: settings.initialized && settings.values.preview.enable

    id: pathViewCore

    on_ReadyChanged: {
        if (_ready) {
            file.onUploadFinished.connect(fileUploadFinished)
            file.onDownloadFinished.connect(fileDownloadFinished)
        }
    }

    function fileUploadFinished() {
        gcodeProgramModel.clear()
        gcodeProgramLoader.load()
        if (_previewEnabled) {
            executePreview()
        }
    }

    function fileDownloadFinished() {
        gcodeProgramModel.clear()
        gcodeProgramLoader.load()
        if (_previewEnabled) {
            executePreview()
        }
    }

    on_PreviewEnabledChanged: {
        if (_previewEnabled
            && (file.remoteFilePath !== "")
            && (file.localFilePath !== "")
            && (file.transferState === ApplicationFile.NoTransfer))
        {
            gcodeProgramModel.clear()
            gcodeProgramLoader.load()
            executePreview()
        }
    }

    function executePreview() {
        if (file.remoteFilePath.split('.').pop() === 'ngc') {   // only open ngc files
            command.openProgram('preview', file.remoteFilePath)
            command.runProgram('preview', 0)
        }
    }

    Service {
        id: previewService
        type: "preview"
    }
    Service {
        id: previewStatusService
        type: "previewstatus"
    }

    PreviewClient {
        property bool _connected: false

        id: previewClient
        statusUri: previewStatusService.uri
        previewUri: previewService.uri
        ready: ((previewService.ready && previewStatusService.ready) || _connected)
        model: gcodeProgramModel
        units: status.synced ? status.config.programUnits : PreviewClient.CanonUnitsInches

        onConnectedChanged: delayTimer.running = true
    }

    Timer { // workaround for binding loop
        id: delayTimer
        interval: 10
        repeat: false
        onTriggered: previewClient._connected = previewClient.connected
    }

    GCodeProgramModel {
        id: gcodeProgramModel
    }

    GCodeProgramLoader {
        id: gcodeProgramLoader
        model: gcodeProgramModel
        localPath: pathViewCore.file.localPath
        remotePath: pathViewCore.file.remotePath
        localFilePath: pathViewCore.file.localFilePath
        onLoadingFailed: console.log("loading file failed: " + localFilePath)
    }

    GCodeSync {
        model: gcodeProgramModel
        status: pathViewCore.status
    }
}
