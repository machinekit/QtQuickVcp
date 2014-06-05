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
import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Window 2.1
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0

/*!
    \qmltype HalSlider
    \inqmlmodule Machinekit.HalRemote.Controls
    \brief Provides a vertical or horizontal slider control.
    \ingroup halremotecontrols

    This component provides a slider combined with a HAL pin. By default
    the type of the HAL pin is float and the direction out.

    Note that if you want to disable the visibility of the minimum and maxium value
    you need to set the \l minimumValueVisible and \l maximumValueVisible properties.

    \qml
    HalSlider {
        id: halSlider
        minimumValueVisible: false
        maximumValueVisible: false
    }
    \endqml

    \sa Slider
*/

Slider {
    /*! This property holds the name of the default HAL pin.
    */
    property string name:               "slider"

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

    /*! This property holds whether the position of the texts
        should be inverted or not.
    */
    property bool   textInverted:       false

    /*! \qmlproperty Text valueText

        This property holds the \l Text used to display the value.
    */
    property alias valueText: valueText

    /*! \qmlproperty Text minimumValueText

        This property holds the \l Text used to display the minimum value.
    */
    property alias minimumValueText: minimumValueText

    /*! \qmlproperty Text maximumValueText

        This property holds the \l Text used to display the maximum value.
    */
    property alias maximumValueText: maximumValueText

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

    id: main

    tickmarksEnabled: true

    HalPin {
        id: pin

        name: main.name
        type: HalPin.Float
        direction: HalPin.Out
    }

    Text {
        id: valueText

        x: (main.orientation === Qt.Horizontal) ? (parent.width - width) * ((main.value - main.minimumValue)/(main.maximumValue - main.minimumValue)) : ((main.textInverted == false) ? -width - Screen.logicalPixelDensity : parent.width)
        y: (main.orientation === Qt.Horizontal) ? ((main.textInverted == false) ? parent.height : -height) : (parent.height - height) * (1 - ((main.value - main.minimumValue)/(main.maximumValue - main.minimumValue)))
        horizontalAlignment: Text.AlignLeft
        text: main.prefix + main.value.toFixed(main.decimals) + main.suffix
    }

    Text {
        id: minimumValueText

        x: (main.orientation == Qt.Horizontal) ? 0 : ((main.textInverted == false) ? parent.width : -width - Screen.logicalPixelDensity)
        y: (main.orientation == Qt.Horizontal) ? ((main.textInverted == false) ? -height : parent.height) : main.height-height
        text: main.prefix + main.minimumValue.toFixed(main.decimals) + main.suffix
    }

    Text {
        id: maximumValueText

        x: (main.orientation == Qt.Horizontal) ? parent.width-width : ((main.textInverted == false) ? parent.width : -width - Screen.logicalPixelDensity)
        y: (main.orientation == Qt.Horizontal) ? ((main.textInverted == false) ? -height : main.height) : 0
        text: main.prefix + main.maximumValue.toFixed(main.decimals) + main.suffix
    }

    BusyIndicator {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: Screen.logicalPixelDensity
        height: Screen.logicalPixelDensity * 4
        width: height
        running: true
        visible: !pin.synced
    }

    Binding { target: main; property: "value"; value: pin.value}
    Binding { target: pin; property: "value"; value: main.value}
}
