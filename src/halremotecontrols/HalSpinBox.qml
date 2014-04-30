import QtQuick 2.0
import QtQuick.Controls 1.1
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0

/*!
    \qmltype HalSpinBox
    \inqmlmodule Machinekit.HalRemote.Controls
    \brief Provides a spin box control.
    \ingroup halremotecontrols

    This component provides a spin box combined with a HAL pin. By default
    the type of the HAL pin is bit and the direction out.

    \qml
    HalSpinBox {
        id: halSpinBox
        name: "spinBox"
    }
    \endqml

    Note that if you require decimal values you will need to set the \l SpinBox::decimals to a non 0 value.

    \qml
    HalSpinBox {
        id: halSpinBox
        name: "spinBox"
        decimals: 2
    }
    \endqml

    \sa SpinBox
*/

SpinBox {
    /*! This property holds the name of the default HAL pin.
    */
    property string name:   "spinBox"

    /*! \qmlproperty HalPin halPin

        This property holds the default HAL pin.
    */
    property alias  halPin: pin

    id: main

    HalPin {
        id: pin

        name: main.name
        type: HalPin.Float
        direction: HalPin.Out
    }

    Binding { target: main; property: "currentIndex"; value: pin.value}
    Binding { target: pin; property: "value"; value: main.currentIndex}
}
