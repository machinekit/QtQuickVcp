import QtQuick 2.1
import QtQuick.Controls 1.1
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0

/*!
    \qmltype HalCheckBox
    \inqmlmodule Machinekit.HalRemote.Controls
    \brief A checkbox with a text label.
    \ingroup halremotecontrols

    This component provides a check box combined with a HAL pin. By
    default the type of the HAL pin is bit and the direction out.

    \qml
    HalCheckBox {
        id: halCheckBox
        name: "checkBox"
    }
    \endqml

    \sa CheckBox
*/

CheckBox {
    /*! This property holds the name of the default HAL pin.
    */
    property string name:   "checkBox"

    /*! \qmlproperty HalPin halPin

        This property holds the default HAL pin.
    */
    property alias  halPin: pin

    id: main

    enabled: pin.direction == HalPin.Out

    HalPin {
        id: pin

        name: main.name
        type: HalPin.Bit
        direction: HalPin.Out
    }

    Binding { target: main; property: "checked"; value: pin.value}
    Binding { target: pin; property: "value"; value: main.checked}
}
