import QtQuick 2.0
import Machinekit.Application 1.0

Item {
    property alias core: object.core
    property alias status: object.status
    property alias settings: object.settings
    property alias command: object.command
    property alias file: object.file
    property alias error: object.error
    property alias mdiHistory: object.mdiHistory

    ApplicationObject {
        id: object
    }
}
