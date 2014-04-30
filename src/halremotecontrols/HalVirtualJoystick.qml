import QtQuick 2.1
import Machinekit.HalRemote 1.0
import Machinekit.Controls 1.0
import Machinekit.HalRemote.Controls 1.0

/*!
    \qmltype HalVirtualJoystick
    \inqmlmodule Machinekit.HalRemote.Controls
    \brief A virtual joystick control.
    \ingroup halremotecontrols

    This component provides a virtual joystick combined with two HAL
    pins. By default the type of the HAL pins is float and the
    direction out. The HAL pins are named \l{name}.x and \l{name}.y.

    \qml
    HalVirtualJoystick {
        id: halVirtualJoystick
        name: "virtualJoystick"
    }
    \endqml

    \sa VirtualJoystick
*/

VirtualJoystick {
    /*! This property holds the base name of the HAL pins.
    */
    property string name:       "joystick"

    /*! \qmlproperty HalPin halPinX

        This property holds the x HAL pin providing the x value of
        the joystick.
    */
    property alias  halPinX:    pinX

    /*! \qmlproperty HalPin halPinY

        This property holds the y HAL pin providing the y value of
        the joystick.
    */
    property alias  halPinY:    pinY

    id: main

    HalPin {
        id: pinX

        name: main.name + ".x"
        type: HalPin.Float
        direction: HalPin.Out
    }

    HalPin {
        id: pinY

        name: main.name + ".y"
        type: HalPin.Float
        direction: HalPin.Out
    }

    Binding { target: pinX; property: "value"; value: main.xValue}
    Binding { target: pinY; property: "value"; value: main.yValue}
}
