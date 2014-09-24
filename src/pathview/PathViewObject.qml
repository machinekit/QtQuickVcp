import QtQuick 2.0
import Machinekit.PathView 1.0

QtObject {
    property var core: null
    property var gcodeProgramLoader: core === null ? {} : core.gcodeProgramLoader
    property var gcodeProgramModel: core === null ? null : core.gcodeProgramModel

    Component.onCompleted: {
        if (core == null)
        {
            try {
                var x = pathViewCore
                core = Qt.binding(function() {return x})
            }
            catch (err) {
            }
        }
    }
}
