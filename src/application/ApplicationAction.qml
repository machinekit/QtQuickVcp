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
