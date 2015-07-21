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
import QtQuick.Window 2.0
import Machinekit.HalRemote 1.0
import Machinekit.Controls 1.0
import Machinekit.HalRemote.Controls 1.0

/*!
    \qmltype HalVirtualJoystick
    \inqmlmodule Machinekit.HalRemote.Controls
    \brief A virtual joystick control.
    \ingroup halremotecontrols

    This component provides a virtual joystick combined with two HAL
    pins. By default the type of the HAL pins is float and the
    direction out. The HAL pins are named \l{name}.x and \l{name}.y.

    \qml
    HalVirtualJoystick {
        id: halVirtualJoystick
        name: "virtualJoystick"
    }
    \endqml

    \sa VirtualJoystick
*/

VirtualJoystick {
    /*! This property holds the base name of the HAL pins.
    */
    property string name:       "joystick"

    /*! \qmlproperty HalPin halPinX

        This property holds the x HAL pin providing the x value of
        the joystick.
    */
    property alias  halPinX:    pinX

    /*! \qmlproperty HalPin halPinY

        This property holds the y HAL pin providing the y value of
        the joystick.
    */
    property alias  halPinY:    pinY

    id: main

    HalPin {
        id: pinX

        name: main.name + ".x"
        type: HalPin.Float
        direction: HalPin.Out
    }

    HalPin {
        id: pinY

        name: main.name + ".y"
        type: HalPin.Float
        direction: HalPin.Out
    }

    BusyIndicator {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: Screen.pixelDensity
        height: Screen.pixelDensity * 4
        width: height
        running: true
        visible: !pinX.synced || !pinY.synced
    }

    Binding { target: pinX; property: "value"; value: main.xValue}
    Binding { target: pinY; property: "value"; value: main.yValue}
    Binding { target: main; property: "xValue"; value: pinX.value}
    Binding { target: main; property: "yValue"; value: pinY.value}
}
