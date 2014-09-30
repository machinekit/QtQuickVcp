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
import Machinekit.Controls 1.0

/*!
    \qmltype VirtualJoystick
    \inqmlmodule Machinekit.Controls
    \brief A virtual joystick control.
    \ingroup machinekitcontrols

    The virtual joystick provides a to simulate a joystick-like control
    for touchscreen displays.

    The controls has two output values called \l xValue and \l{yValue}.
    The possible output values are - \l maximumValue to \l maximumValue whereas
    0 is the value in the center of the joystick.

    \qml
    VirtualJoystick {
        id: virtualJoystick
    }
    \endqml
*/

Rectangle {

    /*! This property holds the maximum alowed value in x and y direction.
        The output value is in the range of  - \c maximumValue to \c{maximumValue}.

        The default value is \c{100}.
    */
    property double maximumValue: 100.0

    /*! This property holds the output value step size.

      The default value is \c{1}.
    */
    property double stepSize: 1.0

    /*! This property holds wheter the joystick should be automacially fall back
        to the center position or not.

        The default value is \c{true}.
    */
    property bool autoCenter: true

    /*! This property holds joystick position in x direction
    */
    property double xValue: 0

    /*! This property holds joystick position in y direction
    */
    property double yValue: 0

    /*! This property holds whether the x axis should be enabled or not.
        The joystick will not move on this axis if disabled.

        The default value is \c{true}.
    */
    property bool xEnabled: true

    /*! This property holds whether the x axis should be enabled or not.
        The joystick will not move on this axis if disabled.

        The default value is \c{true}.
    */
    property bool yEnabled: true

    /*! This property holds whether the pattern should be visible or not.

        The default value is \c{true}.
    */
    property bool patternVisible: true

    /*! \internal */
    readonly property double _centeredX: main.width/2 - control.width/2

    /*! \internal */
    readonly property double _centeredY: main.height/2 - control.height/2

    /*! \internal */
    // Math.sqrt(Math.pow(maxRadius, 2)-Math.pow(y, 2))
    readonly property double _maxX: main.width - control.width

    /*! \internal */
    readonly property double _maxY: main.height - control.height

    /*! \internal */
    readonly property double _maxControlX: control.width/2 - main.width/2

    /*! \internal */
    readonly property double _maxControlY: control.height/2 - main.height/2

    /*! \internal */
    readonly property double _biggerSide: (height > width) ? height : width

    /*! \internal */
    readonly property double _smallerSide: (height > width) ? width : height

    /*! \internal */
    function calculateNewX()
    {
        if (xEnabled)
        {
            var newControlX = 0

            newControlX = controlArea.mouseX - control.width/2
            if (newControlX > _maxX)
            {
                newControlX = _maxX
            }
            else if (newControlX < 0)
            {
                newControlX = 0
            }

            control.x = newControlX
        }
    }

    /*! \internal */
    function calculateNewY()
    {
        if (yEnabled)
        {
            var newControlY = 0

            newControlY = controlArea.mouseY - control.height/2
            if (newControlY > _maxY)
            {
                newControlY = _maxY
            }
            else if (newControlY < 0)
            {
                newControlY = 0
            }

            control.y = newControlY
        }
    }

    id: main

    width: implicitWidth; height: implicitHeight
    implicitWidth: xEnabled ? 150: 30; implicitHeight: yEnabled ? 150 : 30
    color: "#00000000"

    Binding {
        target: main
        property: "xValue";
        value: (Math.floor(((control.x + _maxControlX) / Math.abs(_maxControlX) * maximumValue) / stepSize) * stepSize);
        when: controlArea.pressed
    }
    Binding {
        target: control;
        property: "x";
        value: xValue / maximumValue * Math.abs(_maxControlX) - _maxControlX;
        when: !controlArea.pressed
    }
    Binding {
        target: main;
        property: "yValue";
        value: (Math.floor(((control.y + _maxControlY) / Math.abs(_maxControlY) * maximumValue) / stepSize) * stepSize);
        when: controlArea.pressed
    }
    Binding {
        target: control;
        property: "y";
        value: main.yValue / main.maximumValue * Math.abs(_maxControlY) - _maxControlY;
        when: !controlArea.pressed
    }

    SystemPalette {
        id: systemPalette;
        colorGroup: enabled ? SystemPalette.Active : SystemPalette.Disabled
    }

    Rectangle {
        id: circle1

        width: parent.width; height: parent.height
        anchors.centerIn: parent
        radius: (width < height) ? width / 2 : height / 2
        clip: true
        color: systemPalette.highlight

        Rectangle {
            id: circle2

            visible: patternVisible
            width: main.width * 0.8; height: main.height * 0.8
            anchors.centerIn: parent
            radius: (width < height) ? width / 2 : height / 2
            clip: true
            color: systemPalette.window

            Rectangle {
                id: circle3

                visible: patternVisible
                width: main.width * 0.6; height: main.height * 0.6
                anchors.centerIn: parent
                radius: (width < height) ? width / 2 : height / 2
                clip: true
                color: systemPalette.highlight
            }
        }
    }

    Rectangle {
        id: control

        property bool movable: false
        property double baseSide: _biggerSide * 0.3
        property double side: (baseSide > _smallerSide) ? _smallerSide * 1.5 : baseSide

        width: side; height: side
        x: _centeredX; y: _centeredY; z: 1
        radius: width / 2
        clip: true
        color: {
            if (!movable)
                systemPalette.dark
            else
                systemPalette.midlight
        }
    }

    Line {
        id: line

        x1: main.width/2; y1: main.height/2
        x2: control.x + control.width/2; y2: control.y + control.height/2
        lineWidth: control.width * 0.4
        color: systemPalette.dark
    }

    MouseArea {
        id: controlArea

        anchors.fill: parent

        onPressed: control.movable = true
        onReleased: {
            control.movable = false
            if (main.autoCenter)
            {
                control.x = _centeredX
                control.y = _centeredY
            }
        }
        onMouseXChanged: {
            if (control.movable)
            {
                calculateNewX()
            }
        }
        onMouseYChanged: {
            if (control.movable)
            {
                calculateNewY()
            }
        }
    }

    /*onYVelocityChanged: {
        console.log("Y:")
        console.log(yVelocity)
    }

    onXVelocityChanged: {
        console.log("X:")
        console.log(xVelocity)
    }*/
}
