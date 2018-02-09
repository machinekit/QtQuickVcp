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
    property alias previewClient: previewClient

    /*! Updates the preview if a file is loaded. */
    function updatePreview() {
        if (file.remoteFilePath !== "file://") {
            d.executePreview();
        }
    }

    /*! Clears the preview part of the \l GCodeProgramModel. */
    function clearPreview() {
        gcodeProgramModel.clearPreview();
    }

    id: pathViewCore

    QtObject {
        id: d
        readonly property bool ready: file.ready
        readonly property bool previewEnabled: settings.initialized && settings.values.preview.enable

        /*! /internal
            Cannot directly connect to slots since the file property is var and not a QObject.
        */
        onReadyChanged: {
            if (ready) {
                file.onUploadFinished.connect(onFileUploadFinished);
                file.onDownloadFinished.connect(onFileDownloadFinished);
                core.onProgramReloaded.connect(onProgramReloaded);
                core.onProgramClosed.connect(onProgramClosed);
            }
            else {
                file.onUploadFinished.disconnect(onFileUploadFinished);
                file.onDownloadFinished.disconnect(onFileDownloadFinished);
                core.onProgramReloaded.disconnect(onProgramReloaded);
                core.onProgramClosed.connect(onProgramClosed);
            }
        }

        function onFileUploadFinished() {
            reloadModelAndPreview();
        }

        function onFileDownloadFinished() {
            reloadModelAndPreview();
        }

        function onProgramReloaded() {
            reloadModelAndPreview();
        }

        function onProgramClosed() {
            pathViewCore.clearPreview();
        }

        onPreviewEnabledChanged: {
            if (previewEnabled
                && (file.remoteFilePath !== "")
                && (file.localFilePath !== "")
                && (file.transferState === ApplicationFile.NoTransfer))
            {
                executePreview();
            }
        }

        function reloadModelAndPreview() {
            gcodeProgramModel.clear();
            gcodeProgramLoader.load();
            if (previewEnabled) {
                executePreview();
            }
        }

        function executePreview() {
            if (file.remoteFilePath.split('.').pop() === 'ngc') {   // only open ngc files
                gcodeProgramModel.clearPreview();
                command.openProgram('preview', file.remoteFilePath);
                command.runProgram('preview', 0);
            }
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
        id: previewClient
        previewstatusUri: previewStatusService.uri
        previewUri: previewService.uri
        ready: ((previewService.ready && previewStatusService.ready) || previewConnectedQueue.output)
        model: gcodeProgramModel
    }

    QueuedConnection {
        id: previewConnectedQueue
        input: previewClient.connected
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
