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

/*!
    \qmltype Led
    \inqmlmodule Machinekit.Controls
    \brief Provides a LED-like control to display binary values.
    \ingroup machinekitcontrols

    By default a the led will be red when the value is true and
    grey when the value is false.

    \qml
    Led {
        id: led
    }
    \endqml

    It is easily possible to change the color and shape of the led. Set the
    \l Rectangle::radius of the led to \c 0 to get a square led.

    \qml
    Led {
        id: led
        radius: 0
        onColor: "blue"
        offColor: "black"
    }
    \endqml

    Furthermore it is possible to make the led blinking when enabled.

    \qml
    Led {
        id: led
        blink: true
        blinkInterval: 1000
    }
    \endqml
*/

Rectangle {
    /*! This property holds the led's current value.

        The default value is \c{false}.
    */
    property bool value: false

    /*! This property inverts the polarity of the led.

        The default value is \c{false}.
    */
    property bool invert: false

    /*! This property holds the led's on color.

        The default value is \c{red}.
    */
    property color onColor: "red"

    /*! This property holds the led's off color.

        The default value is \c{darkGrey}.
    */
    property color offColor: "darkGrey"

    /*! This property holds whether the led should blink or not.

        The default value is \c{false}.
    */
    property bool blink: false

    /*! This property holds the blink interval of the led in ms when \l blink is set to \c true.

        The default value is \c{false}.
    */
    property int blinkInterval: 500

    /*! This property holds whether the led should have a small "shine" or not.

        The default value is \c{true}.
    */
    property bool shine: true

    id: main

    width: 30
    height: 30
    implicitWidth: 30
    implicitHeight: 30
    border.width: 2
    radius: width/2
    border.color: "black"
    color: ((value ^ invert) && (helpItem.blinkHelper || !blink)) ? onColor : offColor
    opacity: enabled ? 1.0 : 0.3

    Rectangle {
        x: parent.width*0.15
        y: parent.width*0.15
        width: parent.width*0.4
        height: width
        radius: width/2
        color: "white"
        opacity: 0.4
        visible: main.shine
    }

    Timer {
        id: blinkTimer
        running: blink
        repeat: true
        interval: blinkInterval
        onTriggered: helpItem.blinkHelper = !helpItem.blinkHelper
    }

    QtObject {
        id: helpItem
        property bool blinkHelper: true
    }
}
