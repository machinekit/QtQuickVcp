import QtQuick 2.0
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0

/*!
    \qmltype HalContainer
    \inqmlmodule Machinekit.HalRemote.Controls
    \brief A container item to enable and disable children.
    \ingroup halremotecontrols

    This component is a container item which enables
    disabling and enabling of it's children using the connected
    HAL pin. By default the type of the HAL pin is bit and the
    direction in.

    The container item can be use to disable certain functionalites
    of on application.

    \qml
    HalContainer {
        id: halContainer
        name: "container"
    }
    \endqml
*/

Item {
    /*! This property holds the name of the default HAL pin.
    */
    property string name:   "container"

    /*! \qmlproperty HalPin halPin

        This property holds the default HAL pin.
    */
    property alias  halPin: pin

    id: main

    width: 200
    height: 200

    HalPin {
        id: pin

        name: main.name
        type: HalPin.Bit
        direction: HalPin.In
        value: false
    }

    Binding { target: main; property: "enabled"; value: pin.value }
}
