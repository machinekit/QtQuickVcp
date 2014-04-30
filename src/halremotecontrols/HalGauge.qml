import QtQuick 2.0
import Machinekit.HalRemote 1.0
import Machinekit.Controls 1.0
import Machinekit.HalRemote.Controls 1.0

/*!
    \qmltype HalGauge
    \inqmlmodule Machinekit.HalRemote.Controls
    \brief Provides a linear gauge control to display data.
    \ingroup halremotecontrols

    This component provides a gauge combined with a HAL pin. By default
    the type of the HAL pin is float and the direction in.

    \qml
    Gauge {
        id: halGauge
        name: "gauge"
    }
    \endqml

    \sa Gauge, HalProgressBar, ProgressBar
*/

Gauge {
    /*! This property holds the name of the default HAL pin.
    */
    property string name:   "gauge"

    /*! \qmlproperty HalPin halPin

        This property holds the default HAL pin.
    */
    property alias  halPin: pin

    id: main

    HalPin {
        id: pin

        name: main.name
        type: HalPin.Float
        direction: HalPin.In
    }

    Binding { target: main; property: "value"; value: pin.value}
}
