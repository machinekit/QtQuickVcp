import QtQuick 2.1
import QtQuick.Controls 1.1
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0

/*!
    \qmltype HalRadioButton
    \inqmlmodule Machinekit.HalRemote.Controls
    \brief A radio button with a text label.
    \ingroup halremotecontrols

    This component provides a radio button combined with a HAL pin. By
    default the type of the HAL pin is bit and the direction in.

    \qml
    HalRadioButton {
        id: halRadioButton
        name: "radioButton"
    }
    \endqml

    \sa RadioButton
*/

RadioButton {
    /*! This property holds the name of the default HAL pin.
    */
    property string name:   "radioButton"


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
        direction: HalPin.In
    }

    Binding { target: main; property: "checked"; value: pin.value}
    Binding { target: pin; property: "value"; value: main.checked}
}
