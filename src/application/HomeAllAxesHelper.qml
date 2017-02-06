import QtQuick 2.0
import Machinekit.Application 1.0

Item {
    property var status: { "synced": false }
    property var command: {"ready": false }
    property bool running: false
    readonly property bool homingOrderDefined: d.ready && (status.config.axis[0].homeSequence !== -1)
    readonly property bool allHomed: d.ready ? d.allHomed() : false

    QtObject {
        id: d
        readonly property bool ready: status.synced && command.ready
        property int currentSequence: 0
        property var waitingAxesList: []
        property var homingAxesList: []
        property var prepareAxesList: []

        function checkHomingStatus() {
             var homing = false;
             var prepare = false;
             var i;
             var axis;
             var list;

             if (status.task.taskState !== ApplicationStatus.TaskStateOn) {
                 running = false;
                 return;
             }

             for (i = (d.prepareAxesList.length-1); i >= 0 ; --i) {
                 axis = d.prepareAxesList[i];
                 if (!status.motion.axis[axis].homing
                         && !status.motion.axis[axis].homed) {
                     prepare = true;
                 }
                 else {
                     list = d.homingAxesList;
                     list.push(axis);
                     d.homingAxesList = list;

                     list = d.prepareAxesList;
                     list.splice(i, 1);
                     d.prepareAxesList = list;
                 }
             }

             if (prepare) {
                 return;
             }

             for (i = 0; i < d.homingAxesList.length; ++i) {
                 axis = d.homingAxesList[i];
                 if (status.motion.axis[axis].homing) {
                     homing = true;
                     break;
                 }
             }

             if (!homing) {
                 d.homingAxesList = [];
                 d.prepareAxesList = [];

                 if (d.waitingAxesList.length === 0)
                 {
                     running = false;
                     return;
                 }

                 var currentSequence = status.config.axis[d.waitingAxesList[0]].homeSequence;
                 for (i = (d.waitingAxesList.length-1); i >= 0 ; --i) {
                     axis = d.waitingAxesList[i];
                     if (status.config.axis[axis].homeSequence === currentSequence)
                     {
                         command.homeAxis(axis);

                         list = d.prepareAxesList;
                         list.push(axis);
                         d.prepareAxesList = list;

                         list = d.waitingAxesList;
                         list.splice(i, 1);
                         d.waitingAxesList = list;
                     }
                 }
             }
         }

        function allHomed() {
            for (var i = 0; i < status.config.axes; ++i) {
                if (!status.motion.axis[i].homed) {
                    return false;
                }
            }
            return true;
        }
    }

    id: root

    function trigger() {
        if (!d.ready || running) {
            return        }

        var axesList = []

        for (var i = 0; i < status.config.axes; ++i) {
            if (status.motion.axis[i].homed) {
                command.unhomeAxis(i);
            }
            if ((axesList.length === 0) ||
                    (status.config.axis[axesList[0]].homeSequence > status.config.axis[i].homeSequence))
            {
                axesList.push(i);
            }
            else {
                axesList.unshift(i);
            }
        }

        axesList.reverse();
        d.waitingAxesList = axesList;
        d.homingAxesList = [];
        d.prepareAxesList = [];
        d.currentSequence = 0;
        running = true;
        d.checkHomingStatus();
    }

    Timer {
        id: checkTimer
        interval: 400
        repeat: true
        running: root.running && d.ready

        onTriggered: d.checkHomingStatus()
    }
}
