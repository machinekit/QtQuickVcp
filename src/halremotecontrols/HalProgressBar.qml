/****************************************************************************
**
** Copyright (C) 2014 Alexander Rössler
** License: LGPL version 2.1
**
** This file is part of QtQuickVcp.
**
** All rights reserved. This program and the accompanying materials
** are made available under the terms of the GNU Lesser General Public License
** (LGPL) version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** Contributors:
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/
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

    /*! \qmlproperty Label valueLabel

        This property holds the \l Label used to display the value.
    */
    property alias  valueLabel:          valueLabel

    /*! \qmlproperty Label minimumValueLabel

        This property holds the \l Label used to display the minimum value.
    */
    property alias  minimumValueLabel:   minimumValueLabel

    /*! \qmlproperty Label maximumValueLabel

        This property holds the \l Label used to display the maximum value.
    */
    property alias  maximumValueLabel:   maximumValueLabel

    /*! \qmlproperty bool valueVisible

        This property holds whether the value should be visible or not.
    */
    property alias valueVisible: valueLabel.visible

    /*! \qmlproperty bool minimumValueVisible

        This property holds whether the minimum value should be visible or not.
    */
    property alias minimumValueVisible: minimumValueLabel.visible

    /*! \qmlproperty bool maximumValueVisible

        This property holds whether the maximum value should be visible or not.
    */
    property alias maximumValueVisible: maximumValueLabel.visible

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

    Label {
        id: valueLabel

        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: main.prefix + main.value.toFixed(decimals) + main.suffix
        visible: true
    }

    Label {
        id: minimumValueLabel

        anchors.fill: parent
        anchors.leftMargin: (main.orientation == Qt.Horizontal) ?  Screen.pixelDensity*1 : 0
        anchors.bottomMargin: (main.orientation == Qt.Horizontal) ? 0 : Screen.pixelDensity*1
        horizontalAlignment: (main.orientation == Qt.Horizontal) ? Text.AlignLeft : Text.AlignHCenter
        verticalAlignment: (main.orientation == Qt.Horizontal) ? Text.AlignVCenter : Text.AlignBottom
        text: main.prefix + main.minimumValue.toFixed(decimals) + main.suffix
        visible: true
    }

    Label {
        id: maximumValueLabel

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
