/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Quick Controls module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
** Modifed by Alexander Rössler <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/

import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Private 1.0

/*!
    \qmltype Dial
    \inqmlmodule Machinekit.Controls
    \brief Dial provides a dial control.
    \ingroup machinekitcontrols

    The Dial item provides a rounded range control (like a speedometer or potentiometer).

    \qml
    Dial {
        id: dial
    }
    \endqml

*/

Item {
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

    /*!
        \qmlproperty real maximumValue

        This property holds the maximum value of the Dial
        The default value is \c{1.0}.
    */
    property alias maximumValue: range.maximumValue

    /*!
        \qmlproperty real minimumValue

        This property holds the minimum value of the Dial.
        The default value is \c{0.0}.
    */
    property alias minimumValue: range.minimumValue

    /*!
        \qmlproperty real Slider::value

        This property holds the current value of the Slider.
        The default value is \c{0.0}.
    */
    property alias value: range.value

    /*!
        \qmlproperty real stepSize

        This property indicates the dial step size.

        A value of 0 indicates that the value of the dial operates in a
        continuous range between \l minimumValue and \l maximumValue.

        Any non 0 value indicates a discrete stepSize. The following example
        will generate a slider with integer values in the range [0-5].

        \qml
        Dial {
            maximumValue: 5.0
            stepSize: 1.0
        }
        \endqml

        The default value is \c{0.0}.
    */
    property alias stepSize: range.stepSize

    /*!
        \qmlproperty bool wrapping

        This property holds whether wrapping is enabled.

        If true, wrapping is enabled; otherwise some space is inserted at the bottom of the dial to separate the ends of the range of valid values.

        If enabled, the arrow can be oriented at any angle on the dial. If disabled, the arrow will be restricted to the upper part of the dial; if it is rotated into the space at the bottom of the dial, it will be clamped to the closest end of the valid range of values.

        By default this property is \c{false}.
    */
    property bool wrapping: false

    /*!
        \qmlproperty bool tickmarksEnabled

        This property indicates whether the Dial should display tickmarks
        at step intervals.

        The default value is \c false.
    */
    property bool tickmarksEnabled: false

    /*!
        \qmlproperty bool activeFocusOnPress

        This property indicates whether the Dial should receive active focus when
        pressed.
    */
    property bool activeFocusOnPress: false

    /* \internal */
    property alias __containsMouse: mouseArea.containsMouse

    id: dial

    width: 100
    height: 100

    Accessible.role: Accessible.Dial
    Accessible.name: value

    RangeModel {
        id: range
        minimumValue: 0.0
        maximumValue: 1.0
        stepSize: 0.0
        value: 0
    }

    MouseArea {
        id: mouseArea
        anchors.fill:parent
        property bool inDrag
        hoverEnabled:true

        onPositionChanged: {
            if (pressed) {
                value = valueFromPoint(mouseX, mouseY);
                inDrag = true;
            }
        }
        onPressed: {
            value = valueFromPoint(mouseX, mouseY);
             if (activeFocusOnPress) {
                 dial.focus = true;
             }
        }

        onReleased:inDrag = false;
        function bound(val) {
            return Math.max(minimumValue, Math.min(maximumValue, val));
        }

        function valueFromPoint(x, y)
        {
            var yy = height/2.0 - y;
            var xx = x - width/2.0;
            var a = (xx || yy) ? Math.atan2(yy, xx) : 0;

            if (a < Math.PI/ -2)
                a = a + Math.PI * 2;

            var dist = 0;
            var minv = minimumValue*100, maxv = maximumValue*100;

            if (minimumValue < 0) {
                dist = -minimumValue;
                minv = 0;
                maxv = maximumValue + dist;
            }

            var r = maxv - minv;
            var v;
            if (wrapping)
                v =  (0.5 + minv + r * (Math.PI * 3 / 2 - a) / (2 * Math.PI));
            else
                v =  (0.5 + minv + r* (Math.PI * 4 / 3 - a) / (Math.PI * 10 / 6));

            if (dist > 0)
                v -= dist;
            return bound(v/100)
        }
    }
    StyleItem {
        anchors.fill: parent
        elementType: "dial"
        hasFocus: dial.focus
        sunken: mouseArea.pressed
        maximum: range.maximumValue * 100
        minimum: range.minimumValue * 100
        value: visualPos * 100
        enabled: dial.enabled
        step: range.stepSize * 100
        activeControl: tickmarksEnabled ? "tick" : ""
        property real visualPos : range.maximumValue - range.value

        Behavior on visualPos {
            enabled: !mouseArea.inDrag
            NumberAnimation {
                duration: 300
                easing.type: Easing.OutSine
            }
        }
    }

    Label {
        id: valueLabel

        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: dial.prefix + dial.value.toFixed(dial.decimals) + dial.suffix
    }

    Label {
        id: maximumValueLabel

        anchors.fill: parent
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignBottom
        text: dial.prefix + dial.maximumValue.toFixed(dial.decimals) + dial.suffix
        visible: false
    }

    Label {
        id: minimumValueLabel

        anchors.fill: parent
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignBottom
        text: dial.prefix + dial.minimumValue.toFixed(dial.decimals) + dial.suffix
        visible: false
    }
}
