import QtQuick 2.0
import Machinekit.Controls 1.0
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0

/*!
    \qmltype HalLogChart
    \inqmlmodule Machinekit.HalRemote.Controls
    \brief A chart for logging data.
    \ingroup halremotecontrols

    This component provides a log chart combined with a HAL pin.
    By default the type of the HAL pin is float and the direction in.

    By default the log chart logs data on every change of the HAL pin

    \qml
    HalLogChart {
        id: halLogChart
        name: "logChart"
    }
    \endqml

    \sa LogChart
*/

LogChart {
    /*! This property holds the name of the default HAL pin.
    */
    property string name:   "logChart"

    /*! \qmlproperty HalPin halPin

        This property holds the default HAL pin.
    */
    property alias  halPin: pin

    id: main

    autoSampling: false

    HalPin {
        id: pin

        name: main.name
        type: HalPin.Float
        direction: HalPin.In

        onValueChanged: {
            main.addData(value)
        }
    }
}
