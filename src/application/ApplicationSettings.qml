import QtQuick 2.0
import Machinekit.Application 1.0

LocalSettings {
    property var status: { "synced": false }
    property bool initialized: false
    property bool _ready: status.synced

    name: "settings"

    id: localSettings
    Component.onCompleted: {
        load()
    }
    Component.onDestruction: {
        save()
    }

    on_ReadyChanged: {
        if (_ready && !initialized) {
            for (var i = 0; i < status.config.axes; ++i) {
                setValue("axis" + i + ".jogVelocity", status.config.defaultVelocity, false)
            }
            setValue("dro.showOffsets", false, false)
            setValue("mdi.history", [], false)
            initialized = true
        }
    }
}
