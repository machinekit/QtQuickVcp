import QtQuick 2.0
import Machinekit.Service 1.0
import Machinekit.PathView 1.0
import Machinekit.Application 1.0

ApplicationItem {
    property alias gcodeProgramModel: gcodeProgramModel
    property alias gcodeProgramLoader: gcodeProgramLoader

    property bool _ready: file.ready

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
        command.openProgram('preview', file.remoteFilePath)
        command.runProgram('preview', 0)
    }

    function fileDownloadFinished() {
        gcodeProgramModel.clear()
        gcodeProgramLoader.load()
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
