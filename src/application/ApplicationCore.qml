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
    property ApplicationFile file: applicationFile
    property ApplicationError error: applicationError
    property ApplicationHelper helper: applicationHelper
    property ApplicationSettings settings: uiSettings
    property MdiHistory mdiHistory: mdiHistory
    property HomeAllAxesHelper homeAllAxesHelper: homeAllAxesHelper
    property Item notifications: null
    property string applicationName: "machinekit"

    id: applicationCore

    Component.onCompleted: {
        status.onTaskChanged.connect(_statusTaskChanged);
        status.onConfigChanged.connect(_statusConfigChanged);
        file.onUploadFinished.connect(_fileUploadFinished);
        error.onMessageReceived.connect(_errorMessageReceived);
        file.onErrorChanged.connect(_fileServiceError);
        status.onErrorStringChanged.connect(_statusServiceError);
        command.onErrorStringChanged.connect(_commandServiceError);
        error.onErrorStringChanged.connect(_errorServiceError);
    }

    QtObject {
        id: d
        property bool ignoreNextFileChange: false // helper to prevent downloading file we just uploaded
    }

    function ignoreNextFileChange() {
        d.ignoreNextFileChange = true;
    }

    function _statusTaskChanged() {
        _checkRemoteFile();
    }

    function _statusConfigChanged() {
        applicationFile.remotePath = "file://" + status.config.remotePath;
        _checkRemoteFile();
    }

    function _checkRemoteFile() {
        var remoteFile = "file://" + status.task.file;
        var remotePath = "file://" + status.config.remotePath;

        if (file.remoteFilePath === remoteFile) {
            return; // file did not change
        }

        if (remotePath === "file://") {
            return; // remote path is invalid
        }

        if (remoteFile === "file://") {
            file.remoteFilePath = remoteFile; // unload program
        }
        else if (remoteFile.indexOf(remotePath) === 0) {
            file.remoteFilePath = remoteFile;
            if (!d.ignoreNextFileChange) {
                file.startDownload(); // only start download when program is open
            }
            else {
                d.ignoreNextFileChange = false;
            }
        }
        else {
            return; // remoteFilePaths stays unchanged (subprogram)
        }
    }

    function _fileUploadFinished() {
        if (status.task.taskMode !== ApplicationStatus.TaskModeAuto) {
            command.setTaskMode('execute', ApplicationCommand.TaskModeAuto);
        }
        if (status.task.file !== "") {
            command.resetProgram('execute');
        }
        var fileName = file.localFilePath.split('/').reverse()[0];
        var newPath = file.remotePath + '/' + fileName;
        file.remoteFilePath = newPath;
        ignoreNextFileChange()
        command.resetProgram('execute');
        command.openProgram('execute', newPath);
    }

    function _localFileChanged() {
        file.startUpload();
    }

    function _errorMessageReceived(type, text) {
        if (notifications !== null) {
            notifications.addNotification(type, text);
        }
    }

    function _fileServiceError() {
        if (file.error !== ApplicationFile.NoError) {
            console.warn("file service error: " + file.errorString);
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
        required: true
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
    }

    FileWatcher {
        id: fileWatcher
        fileUrl: enabled ? applicationFile.localFilePath : ""
        enabled: applicationFile.transferState === ApplicationFile.NoTransfer
        onFileChanged: _localFileChanged()
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
