import QtQuick 2.0
import Machinekit.Application 1.0

ApplicationItem {
    property var status: {"synced": false}
    property var command: {"ready": false}
    property bool running: false

    property bool _ready: status.synced && command.ready
    property int _currentSequence: 0
    property var _waitingAxesList: []
    property var _homingAxesList: []
    property var _prepareAxesList: []

    id: root

    function trigger() {
        if (!_ready || running) {
            return
        }

        var axesList = []

        for (var i = 0; i < status.config.axes; ++i) {
            if (status.motion.axis[i].homed) {
                command.unhomeAxis(i)
            }
            if ((axesList.length == 0) ||
                    (status.config.axis[axesList[0]].homeSequence > status.config.axis[i].homeSequence))
            {
                axesList.push(i)
            }
            else {
                axesList.unshift(i)
            }
        }

        axesList.reverse()
        _waitingAxesList = axesList
        _homingAxesList = []
        _prepareAxesList = []
        _currentSequence = 0
        running = true
        _check()
    }

    function _check() {
        var homing = false
        var prepare = false
        var i
        var axis
        var list

        if (status.task.taskState !== ApplicationStatus.TaskStateOn) {
            running = false
            return
        }

        for (i = (_prepareAxesList.length-1); i >= 0 ; --i) {
            axis = _prepareAxesList[i]
            if (!status.motion.axis[axis].homing
                    && !status.motion.axis[axis].homed) {
                prepare = true
            }
            else {
                list = _homingAxesList
                list.push(axis)
                _homingAxesList = list

                list = _prepareAxesList
                list.splice(i, 1)
                _prepareAxesList = list
            }
        }

        if (prepare) {
            return
        }

        for (i = 0; i < _homingAxesList.length; ++i) {
            axis = _homingAxesList[i]
            if (status.motion.axis[axis].homing) {
                homing = true
                break
            }
        }

        if (!homing) {
            _homingAxesList = []
            _prepareAxesList = []

            if (_waitingAxesList.length == 0)
            {
                running = false
                return
            }

            var currentSequence = status.config.axis[_waitingAxesList[0]].homeSequence
            for (i = (_waitingAxesList.length-1); i >= 0 ; --i) {
                axis = _waitingAxesList[i]
                if (status.config.axis[axis].homeSequence === currentSequence)
                {
                    command.homeAxis(axis)

                    list = _prepareAxesList
                    list.push(axis)
                    _prepareAxesList = list

                    list = _waitingAxesList
                    list.splice(i, 1)
                    _waitingAxesList = list
                }
            }
        }
    }

    Timer {
        id: checkTimer
        interval: 400
        repeat: true
        running: root.running && _ready

        onTriggered: _check()
    }
}
