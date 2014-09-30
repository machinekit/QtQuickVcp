import QtQuick 2.0
import Machinekit.Service 1.0
import Machinekit.Application 1.0

QtObject {
    property ApplicationStatus status: applicationStatus //{"synced": false}
    property ApplicationCommand command: applicationCommand //{"connected": false}
    property ApplicationFile file: applicationFile //{""}
    property ApplicationError error: applicationError //{"connected": false}
    property ApplicationSettings settings: uiSettings //{"initialized": false}
    property MdiHistory mdiHistory: mdiHistory
    property Item notifications: null

    id: applicationCore

    Component.onCompleted: {
        status.onTaskChanged.connect(statusTaskChanged)
        status.onConfigChanged.connect(statusConfigChanged)
        file.onUploadFinished.connect(fileUploadFinished)
        error.onMessageReceived.connect(errorMessageReceived)
        file.onErrorChanged.connect(fileError)
        status.onErrorChanged.connect(statusError)
        command.onErrorChanged.connect(commandError)
        error.onErrorChanged.connect(errorError)
    }

    function statusTaskChanged() {
        checkFile()
    }

    function statusConfigChanged() {
        applicationFile.remotePath = "file://" + status.config.remotePath
        checkFile()
    }

    function checkFile() {
        var remoteFile = "file://" + status.task.file
        var remotePath = "file://" + status.config.remotePath
        if ((status.config.remotePath !== "")
                && (remoteFile !== "file://")
                && (remoteFile.indexOf(remotePath) === 0)
                && (file.remoteFilePath != remoteFile))
        {

            file.remoteFilePath = remoteFile
            file.startDownload()
        }
    }

    function fileUploadFinished() {
        if (status.task.taskMode !== ApplicationStatus.TaskModeAuto)
            command.setTaskMode('execute', ApplicationCommand.TaskModeAuto)
        if (status.task.file !== "") {
            command.resetProgram('execute')
        }
        command.openProgram('execute', file.remoteFilePath)
    }

    function errorMessageReceived(type, text) {
        if (notifications != null) {
            notifications.addNotification(type, text)
        }
    }

    function fileError() {
        console.log("file error: " + file.errorString)
    }

    function statusError() {
        console.log("status error: " + status.errorString)
    }

    function commandError() {
        console.log("command error: " + command.errorString)
    }

    function errorError() {
        console.log("error error: " + error.errorString)
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
        ready: (statusService.ready || connected)
    }

    ApplicationCommand {
        id: applicationCommand
        commandUri: commandService.uri
        ready: (commandService.ready || connected)
    }

    ApplicationError {
        id: applicationError
        errorUri: errorService.uri
        ready: (errorService.ready || connected)
    }

    ApplicationFile {
        id: applicationFile
        uri: fileService.uri
        ready: fileService.ready
    }

    ApplicationSettings {
        id: uiSettings
        status: applicationStatus
    }

    MdiHistory {
        id: mdiHistory
        settings: uiSettings
    }
}
