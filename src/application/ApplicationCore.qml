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
import Machinekit.Application 1.0

Item {
    property ApplicationStatus status: applicationStatus
    property ApplicationCommand command: applicationCommand
    property QtObject file: applicationFile.ready ? applicationFile : applicationFile2
    property ApplicationError error: applicationError
    property ApplicationHelper helper: applicationHelper
    property ApplicationSettings settings: uiSettings
    property MdiHistory mdiHistory: mdiHistory
    property HomeAllAxesHelper homeAllAxesHelper: homeAllAxesHelper
    property Item notifications: null
    property string applicationName: "machinekit"

    signal programReloaded()
    signal programClosed()

    id: applicationCore

    Component.onCompleted: {
        error.onMessageReceived.connect(_errorMessageReceived);
        file.onErrorChanged.connect(_fileServiceError);
        status.onErrorStringChanged.connect(_statusServiceError);
        command.onErrorStringChanged.connect(_commandServiceError);
        error.onErrorStringChanged.connect(_errorServiceError);
    }

    QtObject {
        id: d
        readonly property string remoteFilePath: "file://" +  status.task.file
        readonly property string remotePath: "file://" + status.config.remotePath
    }

    function executeProgram(remoteFilePath) {
        if (status.task.taskMode !== ApplicationStatus.TaskModeAuto) {
            command.setTaskMode('execute', ApplicationCommand.TaskModeAuto);
        }
        command.resetProgram('execute');
        command.openProgram('execute', remoteFilePath);
    }

    function reloadProgram() {
        executeProgram(file.remoteFilePath);
        programReloaded();
    }

    function closeProgram() {
        if (status.task.taskMode !== ApplicationStatus.TaskModeAuto) {
            command.setTaskMode('execute', ApplicationCommand.TaskModeAuto);
        }
        command.resetProgram('execute');
        programClosed();
    }

    function _errorMessageReceived(type, text) {
        if (notifications !== null) {
            notifications.addNotification(type, text);
        }
    }

    function _fileServiceError() {
        if (applicationCore.file.error !== ApplicationFile2.NoError) {
            console.warn("file service error: " + applicationCore.file.errorString);
            file.clearError();  // ignore errors from FTP
        }
    }

    function _statusServiceError(note) {
        if (note !== "") {
            console.warn("status service error: " + note);
        }
    }

    function _commandServiceError(note) {
        if (note !== "") {
            console.warn("command service error: " + note);
        }
    }

    function _errorServiceError(note) {
        if (note !== "") {
            console.warn("error service error: " + note);
        }
    }

    Service {
        id: statusService
        type: "status"
        required: true
    }
    Service {
        id: commandService
        type: "command"
        required: true
    }
    Service {
        id: errorService
        type: "error"
        required: true
    }
    Service {
        id: fileService
        type: "file"
        required: false
    }
    Service {
        id: file2Service
        type: "file2"
        required: false
    }

    ApplicationStatus {
        id: applicationStatus
        statusUri: statusService.uri
        ready: (statusService.ready || statusSyncedQueue.output)
    }

    QueuedConnection {
        id: statusSyncedQueue
        input: applicationStatus.synced
    }

    ApplicationCommand {
        id: applicationCommand
        commandUri: commandService.uri
        ready: (commandService.ready || commandConnectedQueue.output)
    }

    QueuedConnection {
        id: commandConnectedQueue
        input: applicationCommand.connected
    }

    ApplicationError {
        id: applicationError
        errorUri: errorService.uri
        ready: (errorService.ready || errorConnectedQueue.output)
    }

    QueuedConnection {
        id: errorConnectedQueue
        input: applicationError.connected
    }

    ApplicationFile {
        id: applicationFile
        uri: fileService.uri
        ready: fileService.ready

        onUploadFinished: {
            var extension = remoteFilePath.split('.').pop();
            fileSyncHandler.ignoreNextChange =  ["ngc", "txt"].indexOf(extension) != -1;
            executeProgram(remoteFilePath);
        }
    }

    ApplicationFile2 {
        id: applicationFile2
        uri: file2Service.uri
        ready: file2Service.ready

        onUploadFinished: {
            var extension = remoteFilePath.split('.').pop();
            fileSyncHandler.ignoreNextChange =  ["ngc", "txt"].indexOf(extension) != -1;
            executeProgram(remoteFilePath);
        }
    }

    ApplicationFileSyncHandler {
        id: fileSyncHandler
        ready: applicationCore.file.ready && applicationStatus.ready && applicationCommand.ready
        remoteFilePath: d.remoteFilePath
        remotePath: d.remotePath
        ignoreNextChange: false

        onStartFileDownload: {
            applicationCore.file.remoteFilePath = filePath;
            applicationCore.file.remotePath = remotePath;  // prevents race condition
            applicationCore.file.startDownload();
        }

        onRemotePathChanged: {
            // need to set remotePath if file is going to be uploaded from local computer
            applicationCore.file.remotePath = d.remotePath;
        }
    }

    FileWatcher {
        id: fileWatcher
        fileUrl: applicationCore.file.localFilePath
        enabled: applicationCore.file.transferState === ApplicationFile2.NoTransfer
        recursive: false

        onFileChanged: {
            // file changes when program is running would cause problems on execution
            if (status.synced && !status.running) {
                applicationCore.file.startUpload();
            }
        }
    }

    ApplicationHelper {
        id: applicationHelper
        status: applicationStatus
    }

    ApplicationSettings {
        id: uiSettings
        status: applicationStatus
        application: applicationName
    }

    MdiHistory {
        id: mdiHistory
        settings: uiSettings
    }

    HomeAllAxesHelper {
        id: homeAllAxesHelper
        status: applicationStatus
        command: applicationCommand
    }
}
