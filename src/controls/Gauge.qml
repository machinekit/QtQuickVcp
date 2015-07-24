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
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Window 2.1
import Machinekit.Controls 1.0

/*!
    \qmltype Gauge
    \inqmlmodule Machinekit.Controls
    \brief Provides a linear gauge control to display data.
    \ingroup machinekitcontrols

    By default the gauge will be colored and have a range from 0.0 to 1.0.

    \qml
    Gauge {
        id: gauge
    }
    \endqml

    Note that if you want to disable the visibility of the minimum and maximum value
    you need to set the \l minimumValueVisible and \l maximumValueVisible properties.

    \qml
    Gauge {
        id: gauge
        minimumValueVisible: false
        maximumValueVisible: false
    }
    \endqml

    You can also use the gauge to visualize values growing in both directions
    from the \l zeroValue.

    \qml
    Gauge {
        id: gauge
        fancy: false
        minimumValue: -1.0
        maximumValue: 1.0
        zeroValue: 0.0
    }
    \endqml
*/

Item {
    /*! Whether the values displayed on the gauge should be inverted or not.
        Without invert the minimum value is either on the left or bottom of
        the gauge depending on the \l orientation.

        By default this is set to \c false.
    */
    property bool invert: false

    /*! \qmlproperty enumeration orientation

        This property holds the orientation of the gauge.

        \list
        \li Qt.Horizontal - Horizontal orientation. (Default)
        \li Qt.Vertical - Vertical orientation.
        \endlist
    */
    property int orientation: Qt.Horizontal

    /*! This property holds the gauge's current value.
        Attempting to change the current value to one outside the minimum-maximum
        range has no effect on the current value.

        The default value is \c{0}.
    */
    property double value: 0.0

    /*! This property is the gauge's maximum value.
        The \l value is clamped to this value.
        If maximumValue is smaller than \l minimumValue, \l minimumValue will be enforced.
        The default value is \c{1}.
    */
    property double maximumValue: 1.0

    /*! This property is the gauge's minimum value.
        The \l value is clamped to this value.
        The default value is \c{0}.
    */
    property double minimumValue: 0.0

    /*! This property is the gauge's zero value.
        If the zero value is inside the range of \l minimumValue and \l maximumValue
        the bar will grow from that value and not from the left (or right) side
        of the gauge. Useful to represent value that may be both positive or negative

        By default this is bound to \l minimumValue.
    */
    property double zeroValue: minimumValue

    /*! \internal */
    property alias  style: progressBar.style

    /*! This property indicates whether the control is being hovered.
    */
    readonly property bool hovered: progressBar.hovered || progressBar2.hovered

    /*! This property holds the text color.
    */
    property color textColor: systemPalette.text

    /*! This property holds the background color.
    */
    property color backgroundColor: systemPalette.base

    /*! This property holds the color of value zone 0. Together with \l z1Color and
      \l z2Color this defines the colors for different zones. For description of
      zones see \l z0BorderValue.

      The default value is \c{green}
    */
    property color z0Color: "green"

    /*! This property holds the color of value zone 1. Together with \l z0Color and
      \l z2Color this defines the colors for different zones. For description of
      zones see \l z0BorderValue.

      The default value is \c{yellow}
    */
    property color z1Color: "yellow"

    /*! This property holds the color value zone 2. Together with \l z0Color and
      \l z1Color this defines the colors for different zones. For description of
      zones see \l z0BorderValue.

      The default value is \c{red}
    */
    property color z2Color: "red"

    /*! This property holds the upper bound of value zone 0. Zone 0 will be filled
      with \l z0Color and smoothly transition to \l z1Color. If this property is
      greather than \l maximumValue only one color zone will be displayed. If
      \l zeroValue is greather than \l minimumValue the color zones will be mirrored
      to the left.

      The default value is \c{0.2}
    */
    property double z0BorderValue: 0.2

    /*! This property holds the upper bound of value zone 1. Zone 1 will be filled
      with \l z1Color and smoothly transition to \l z2Color. If this property is
      greather than \l maximumValue only two color zone will be displayed. If
      \l zeroValue is greather than \l minimumValue the color zones will be mirrored
      to the left.

      The default value is \c{0.8}
    */
    property double z1BorderValue: 0.8

    /*! This property indicates the amount of decimals.
      Note that if you enter more decimals than specified, they will
      be truncated to the specified amount of decimal places.

      The default value is \c{2}.
    */
    property int decimals: 2

    /*! The prefix for the value. I.e "$"
    */
    property string prefix: ""

    /*! The suffix for the value. I.e "cm"
    */
    property string suffix: ""

    /*! Corner radius of the gauge.
    */
    property int radius: Screen.pixelDensity*0.7

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

    /*! This property holds whether the the gauge should be styled fancy
        or not. Fancy means that the value zones and colors are used for
        coloring the bar. If fancy is set to false the bar will be colored
        and styled in native system colors.

        By default this is set to \c true.
    */
    property bool fancy: true


    id: main
    width: implicitWidth
    height: implicitHeight
    implicitHeight: (orientation == Qt.Horizontal) ? valueLabel.implicitHeight * 1.25 : progressBar.implicitWidth
    implicitWidth: (orientation == Qt.Horizontal) ? progressBar.implicitWidth : valueLabel.implicitHeight * 1.25

    SystemPalette {
        id: systemPalette;
        colorGroup: main.enabled ? SystemPalette.Active : SystemPalette.Disabled
    }

    Component {
        id: defaultStyle
        ProgressBarStyle {
        }
    }

    Component {
        id: fancyStyle
        ProgressBarStyle {

            progress: Rectangle {
                radius: main.radius
                border.color: systemPalette.shadow
                color: "#00000000"
                clip: true
                z: 1
                Rectangle {
                    radius: parent.radius
                    rotation:       -90
                    width:          parent.parent.parent.height
                    height:         parent.parent.parent.width
                    anchors.centerIn: parent
                    anchors.horizontalCenterOffset: parent.parent.parent.width/2-parent.width/2
                    z: 0
                    gradient: Gradient {
                        GradientStop {
                            position: 0.00
                            color: main.z0Color
                        }
                        GradientStop {
                            position: (main.z0BorderValue-main.zeroValue)/(main.maximumValue-main.zeroValue)
                            color: main.z1Color
                        }
                        GradientStop {
                            position: (main.z1BorderValue-main.zeroValue)/(main.maximumValue-main.zeroValue)
                            color: main.z2Color
                        }
                    }
                    border.color: parent.border.color
                }
            }

            background: Rectangle {
                radius: main.radius
                border.color: systemPalette.shadow
                color: main.backgroundColor
            }
        }
    }

    Item {
        id: rotationItem
        rotation:       ((main.orientation == Qt.Horizontal) ?  ((main.invert == false) ? 0 : 180) : ((main.invert == false) ? 270 : 90))
        width:          ((rotation === 90) || (rotation === 270)) ? main.height : main.width
        height:         ((rotation === 90) || (rotation === 270)) ? main.width : main.height
        anchors.centerIn: parent

        ProgressBar {
            id: progressBar

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width * (main.zeroValue-main.minimumValue) / (main.maximumValue-main.minimumValue)
            rotation: 180
            style: (main.fancy) ? fancyStyle : defaultStyle
            minimumValue: 0.0
            maximumValue: 1.0
            value: (main.value < main.zeroValue) ? (main.zeroValue-main.value)/Math.max(main.zeroValue-main.minimumValue,0.0000001) : 0.0
        }

        ProgressBar {
            id: progressBar2

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: progressBar.right
            anchors.right: parent.right
            style: progressBar.style
            minimumValue: 0.0
            maximumValue: 1.0
            value: (main.value >= main.zeroValue) ? (main.value-main.zeroValue)/(main.maximumValue-main.zeroValue) : 0.0
            indeterminate: progressBar.indeterminate
        }
    }

    Label {
        id: valueLabel

        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: main.prefix + main.value.toFixed(decimals) + main.suffix
        color: main.textColor
        visible: true
    }

    Label {
        id: minimumValueLabel

        anchors.fill: parent
        anchors.rightMargin: ((main.orientation == Qt.Horizontal) && (main.invert != false)) ? Screen.pixelDensity*1 : 0
        anchors.leftMargin: ((main.orientation == Qt.Horizontal) && (main.invert == false)) ? Screen.pixelDensity*1 : 0
        anchors.topMargin:   (main.orientation == Qt.Horizontal) ? 0 : ((main.invert != false) ? Screen.pixelDensity*1 : 0)
        anchors.bottomMargin: (main.orientation == Qt.Horizontal) ? 0 : ((main.invert == false) ? Screen.pixelDensity*1 : 0)
        horizontalAlignment: (main.orientation == Qt.Horizontal) ? ((main.invert != false) ? Text.AlignRight : Text.AlignLeft) : Text.AlignHCenter
        verticalAlignment: (main.orientation == Qt.Horizontal) ? Text.AlignVCenter : ((main.invert != false) ? Text.AlignTop : Text.AlignBottom)
        text: main.prefix + main.minimumValue.toFixed(decimals) + main.suffix
        color: main.textColor
        visible: true
    }

    Label {
        id: maximumValueLabel

        anchors.fill: parent
        anchors.rightMargin: ((main.orientation == Qt.Horizontal) && (main.invert == false)) ? Screen.pixelDensity*1 : 0
        anchors.leftMargin: ((main.orientation == Qt.Horizontal) && (main.invert != false)) ? Screen.pixelDensity*1 : 0
        anchors.topMargin:   (main.orientation == Qt.Horizontal) ? 0 : ((main.invert == false) ? Screen.pixelDensity*1 : 0)
        anchors.bottomMargin: (main.orientation == Qt.Horizontal) ? 0 : ((main.invert != false) ? Screen.pixelDensity*1 : 0)
        horizontalAlignment: (main.orientation == Qt.Horizontal) ? ((main.invert == false) ? Text.AlignRight : Text.AlignLeft) : Text.AlignHCenter
        verticalAlignment: (main.orientation == Qt.Horizontal) ? Text.AlignVCenter : ((main.invert == false) ? Text.AlignTop : Text.AlignBottom)
        text: main.prefix + main.maximumValue.toFixed(decimals) + main.suffix
        color: main.textColor
        visible: true
    }
}
