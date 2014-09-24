import QtQuick 2.0
import Machinekit.Application 1.0

QtObject {
    property var settings: {"initialized": false}
    property var model: []
    property bool _ready: settings.initialized

    id: mdiHistory

    on_ReadyChanged: {
        if (_ready) {
            _update()
            settings.onValuesChanged.connect(_update)
        }
        else {
            settings.onValuesChanged.disconnect(_update)
        }
    }

    function _update() {
        model = settings.value("mdi.history")
        if (model == null) {
            model = []
        }
    }

    function add(command) {
        var tmpModel = model
        tmpModel.push({"command": command})
        settings.setValue("mdi.history", tmpModel)
    }

    function remove(index) {
        var tmpModel = model
        model.splice(index, 1)
        settings.setValue("mdi.history", tmpModel)
    }

    function insert(index, command) {
        var tmpModel = model
        tmpModel.splice(index, 0, command)
        settings.setValue("mdi.history", tmpModel)
    }

    function clear() {
        settings.setValue("mdi.history", [])
    }
}
