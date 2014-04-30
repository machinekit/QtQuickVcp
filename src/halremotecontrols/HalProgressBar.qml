import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Window 2.1
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0

/*!
    \qmltype HalProgressBar
    \inqmlmodule Machinekit.HalRemote.Controls
    \brief A progress indicator.
    \ingroup halremotecontrols

    This component provides a progress bar combined with a HAL pin. By default
    the type of the HAL pin is float and the direction in.

    In addition to the default HAL pin the progress bar controls has
    an additional \l{name}.scale pin. This HAL pin hase the type float
    and direction in. It determines the maximum value of the progress
    bar.

    \qml
    HalProgressBar {
        id: halProgressBar
        name: "progressBar"
    }
    \endqml

    Note that if you want to disable the visibility of the minimum and maxium value
    you need to set the \l minimumValueVisible and \l maximumValueVisible properties.

    \code
    HalProgressBar {
        id: halProgressBar
        name: "progressBar"
        minimumValueVisible: false
        maximumValueVisible: false
    }
    \endcode

    \sa ProgressBar, HalGauge, Gauge
*/

ProgressBar {
    /*! This property holds the name of the default HAL pin.
    */
    property string name:               "progressBar"

    /*! This property indicates the amount of decimals.
      Note that if you enter more decimals than specified, they will
      be truncated to the specified amount of decimal places.

      The default value is \c{2}.
    */
    property int    decimals:           2

    /*! The prefix for the value. I.e "$"
    */
    property string prefix:             ""

    /*! The suffix for the value. I.e "cm"
    */
    property string suffix:             ""

    /*! \qmlproperty Text valueText

        This property holds the \l Text used to display the value.
    */
    property alias  valueText:          valueText

    /*! \qmlproperty Text minimumValueText

        This property holds the \l Text used to display the minimum value.
    */
    property alias  minimumValueText:   minimumValueText

    /*! \qmlproperty Text maximumValueText

        This property holds the \l Text used to display the maximum value.
    */
    property alias  maximumValueText:   maximumValueText

    /*! \qmlproperty bool valueVisible

        This property holds whether the value should be visible or not.
    */
    property alias valueVisible: valueText.visible

    /*! \qmlproperty bool minimumValueVisible

        This property holds whether the minimum value should be visible or not.
    */
    property alias minimumValueVisible: minimumValueText.visible

    /*! \qmlproperty bool maximumValueVisible

        This property holds whether the maximum value should be visible or not.
    */
    property alias maximumValueVisible: maximumValueText.visible

    /*! \qmlproperty HalPin halPin

        This property holds the default HAL pin.
    */
    property alias  halPin:             pin

    /*! \qmlproperty HalPin halPinScale

        This property holds the scale HAL pin.
    */
    property alias  halPinScale:        pinScale

    id: main

    maximumValue: pinScale.value    // can be overwritten

    HalPin {
        id: pin

        name: main.name
        type: HalPin.Float
        direction: HalPin.In
    }

    HalPin {
        id: pinScale

        name: main.name + ".scale"
        type: HalPin.Float
        direction: HalPin.In
    }

    Text {
        id: valueText

        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: main.prefix + main.value.toFixed(decimals) + main.suffix
        visible: true
    }

    Text {
        id: minimumValueText

        anchors.fill: parent
        anchors.leftMargin: (main.orientation == Qt.Horizontal) ?  Screen.pixelDensity*1 : 0
        anchors.bottomMargin: (main.orientation == Qt.Horizontal) ? 0 : Screen.pixelDensity*1
        horizontalAlignment: (main.orientation == Qt.Horizontal) ? Text.AlignLeft : Text.AlignHCenter
        verticalAlignment: (main.orientation == Qt.Horizontal) ? Text.AlignVCenter : Text.AlignBottom
        text: main.prefix + main.minimumValue.toFixed(decimals) + main.suffix
        visible: true
    }

    Text {
        id: maximumValueText

        anchors.fill: parent
        anchors.rightMargin: (main.orientation == Qt.Horizontal) ? Screen.pixelDensity*1 : 0
        anchors.topMargin:   (main.orientation == Qt.Horizontal) ? 0 : Screen.pixelDensity*1
        horizontalAlignment: (main.orientation == Qt.Horizontal) ? Text.AlignRight : Text.AlignHCenter
        verticalAlignment: (main.orientation == Qt.Horizontal) ? Text.AlignVCenter : Text.AlignTop
        text: main.prefix + main.maximumValue.toFixed(decimals) + main.suffix
        visible: true
    }

    Binding { target: main; property: "value"; value: pin.value}
}
