import QtQuick 2.0
import Machinekit.Application 1.0
import Machinekit.PathView 1.0

QtObject {
    property var status: {"synced": false}
    property var model: undefined

    property int _lastLine: 1
    property bool _ready: status.synced

    on_ReadyChanged: {
        if (_ready) {
            status.onMotionChanged.connect(updateLine)
        }
        else {
            status.onMotionChanged.disconnect(updateLine)
        }
    }

    function updateLine() {
        if (_ready) {
            var file = status.task.file
            var currentLine = status.motion.motionLine + 1
            if (_lastLine > currentLine) {
                for (var line = 1; line <= _lastLine; ++line) {
                    model.setData(file, line, false, GCodeProgramModel.ExecutedRole)
                    model.setData(file, line, false, GCodeProgramModel.ActiveRole)
                }
                _lastLine = currentLine
            }

            for (var line = _lastLine; line < currentLine; ++line) {
                model.setData(file, line, true, GCodeProgramModel.ExecutedRole)
                model.setData(file, line, false, GCodeProgramModel.ActiveRole)
            }

            model.setData(file, currentLine, true, GCodeProgramModel.ActiveRole)

            _lastLine = currentLine
        }
    }
}
