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

Item {
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

    /*! \qmlproperty Label valueLabel

        This property holds the \l Label used to display the value.
    */
    property alias valueLabel: valueLabel

    /*! \qmlproperty Label minimumValueLabel

        This property holds the \l Label used to display the minimum value.
    */
    property alias minimumValueLabel: minimumValueLabel

    /*! \qmlproperty Label maximumValueLabel

        This property holds the \l Label used to display the maximum value.
    */
    property alias maximumValueLabel: maximumValueLabel

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

    /*!
        \qmlproperty bool activeFocusOnPress

        This property indicates whether the Slider should receive active focus when
        pressed.
    */
    property alias activeFocusOnPress: slider.activeFocusOnPress

    /*!
        \qmlproperty bool hovered

        This property indicates whether the control is being hovered.
    */
    property alias hovered: slider.hovered

    /*!
        \qmlproperty real maximumValue

        This property holds the maximum value of the Slider
        The default value is \c{1.0}.
    */
    property alias maximumValue: slider.maximumValue

    /*!
        \qmlproperty real minimumValue

        This property holds the minimum value of the Slider.
        The default value is \c{0.0}.
    */
    property alias minimumValue: slider.minimumValue

    /*!
        \qmlproperty enumeration orientation

        This property holds the layout orientation of the slider.
        The default value is \c Qt.Horizontal.
    */
    property alias orientation: slider.orientation

    /*!
        \qmlproperty bool pressed

        This property indicates whether the slider handle is being pressed.
    */
    property alias pressed: slider.pressed

    /*!
        \qmlproperty real stepSize

        This property indicates the slider step size.

        A value of 0 indicates that the value of the slider operates in a
        continuous range between \l minimumValue and \l maximumValue.

        Any non 0 value indicates a discrete stepSize. The following example
        will generate a slider with integer values in the range [0-5].

        \qml
        HalSlider {
            maximumValue: 5.0
            stepSize: 1.0
        }
        \endqml

        The default value is \c{0.0}.
    */
    property alias stepSize: slider.stepSize

    /*! \qmlproperty Component style */
    property alias style: slider.style

    /*!
        \qmlproperty bool tickmarksEnabled

        This property indicates whether the Slider should display tickmarks
        at step intervals.

        The default value is \c false.
    */
    property alias tickmarksEnabled: slider.tickmarksEnabled

    /*!
        \qmlproperty bool updateValueWhileDragging

        This property indicates whether the current \l value should be updated while
        the user is moving the slider handle, or only when the button has been released.
        This property could for instance be modified if changing the slider value would turn
        out to be too time consuming.

        The default value is \c true.
    */
    property alias updateValueWhileDragging: slider.updateValueWhileDragging

    /*!
        \qmlproperty real value

        This property holds the current value of the Slider.
        The default value is \c{0.0}.
    */
    property alias value: slider.value


    /*! \internal */
    property bool __horizontal: orientation === Qt.Horizontal

    /*! \internal */
    property bool __inverted: textInverted === true


    id: main

    width: __horizontal ? 200 : 90
    height: __horizontal ? 60 : 200
    implicitWidth: slider.implicitWidth + (!__horizontal ? 0 : (valueVisible ? valueLabel.implicitWidth : 0) + ((minimumValueVisible || maximumValueVisible) ? minimumValueLabel.implicitWidth : 0))
    implicitHeight: slider.implicitHeight + (__horizontal ? (valueVisible ? valueLabel.implicitHeight : 0) + ((minimumValueVisible || maximumValueVisible) ? minimumValueLabel.implicitHeight : 0) + valueLabel.implicitHeight * 0.7 : 0)

    HalPin {
        id: pin

        name: main.name
        type: HalPin.Float
        direction: HalPin.Out
    }

    Label {
        id: valueLabel

        x: __horizontal ? (main.width - width) * ((main.value - main.minimumValue)/(main.maximumValue - main.minimumValue)) : (!__inverted ? 0 : main.width - width)
        y: __horizontal ? (!__inverted ? 0 : parent.height-height) : (main.height - height) * (1 - ((main.value - main.minimumValue)/(main.maximumValue - main.minimumValue)))
        horizontalAlignment: Text.AlignLeft
        text: main.prefix + main.value.toFixed(main.decimals) + main.suffix
    }

    Label {
        id: minimumValueLabel

        x: __horizontal ? 0 : (!__inverted ? main.width - width : 0)
        y: __horizontal ? (!__inverted ? parent.height-height : 0) : main.height-height
        text: main.prefix + main.minimumValue.toFixed(main.decimals) + main.suffix
    }

    Label {
        id: maximumValueLabel

        x: __horizontal ? main.width-width : (!__inverted ? main.width - width : 0)
        y: __horizontal ? (!__inverted ? parent.height-height : 0) : 0
        text: main.prefix + main.maximumValue.toFixed(main.decimals) + main.suffix
    }

    Slider {
        id: slider

        width: __horizontal ? main.width : implicitWidth
        height: !__horizontal ? main.height : implicitHeight
        anchors.centerIn: main
        tickmarksEnabled: true
    }

    BusyIndicator {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: Screen.pixelDensity
        height: Screen.pixelDensity * 4
        width: height
        running: true
        visible: !pin.synced
    }

    Binding { target: slider; property: "value"; value: pin.value}
    Binding { target: pin; property: "value"; value: slider.value}
}
