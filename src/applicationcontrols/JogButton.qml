import QtQuick 2.0
import QtQuick.Controls 1.2
import Machinekit.Application 1.0
import Machinekit.Application.Controls 1.0

Button {
    property alias core: jogAction.core
    property alias status: jogAction.status
    property alias command: jogAction.command
    property alias axis: jogAction.axis
    property alias distance: jogAction.distance
    property int direction: 1

    function _toggle(onOff) {
        var velocity = jogAction.settings.value("axis" + axis + ".jogVelocity")
        if (onOff) {
            if (jogAction.distance === 0) {
                jogAction.velocity = velocity * direction
                jogAction.trigger()
            }
        }
        else if (jogAction.distance === 0) {
            jogAction.velocity = 0
            jogAction.trigger()
        }
        else {
            jogAction.velocity = velocity * direction
            jogAction.trigger()
        }
    }

    text: (direction == 1) ? "+" : "-"
    enabled: jogAction.enabled

    onPressedChanged: {
        if (!checkable)
            _toggle(pressed)
    }
    onCheckedChanged: {
        if (checkable)
         _toggle(checked)
    }

    JogAction {
        id: jogAction
    }
}
