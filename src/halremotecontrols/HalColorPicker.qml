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
import QtQuick.Window 2.0
import Machinekit.Controls 1.0
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0

/*!
    \qmltype HalColorPicker
    \inqmlmodule Machinekit.HalRemote.Controls
    \brief Provides a beautiful color picker.
    \ingroup halremotecontrols

    This component provides a color picker combined with a HAL pin.

    The color picker controls provides 7 HAL pins named \l{name} +
    \c{.r}, \c{.g}, \c{.b}, \c{.a}, \c{.h}, \c{.s} and \c{.v}.
    By default the type of the r,g,b and a HAL pins is U32 and
    the type of the h,s, and v HAL pins is float. The direction of
    all HAL pins is out.

    \qml
    HalButton {
        id: halButton
        name: "button"
    }
    \endqml

    \sa ColorPicker
*/

ColorPicker {
    /*! This property holds the base name for the HAL pins.
    */
    property string name: "colorPicker"

    /*! \qmlproperty HalPin halPinR

        This property holds the r HAL pin providing the red channel
        of the color.
    */
    property alias  halPinR: pinR

    /*! \qmlproperty HalPin halPinG

        This property holds the g HAL pin providing the green channel
        of the color.
    */
    property alias  halPinG: pinG

    /*! \qmlproperty HalPin halPinB

        This property holds the b HAL pin providing the blue channel
        of the color.
    */
    property alias  halPinB: pinB

    /*! \qmlproperty HalPin halPinA

        This property holds the a HAL pin providing the alpha channel
        of the color.
    */
    property alias  halPinA: pinA

    /*! \qmlproperty HalPin halPinH

        This property holds the h HAL pin providing the hue compoent
        of the color.
    */
    property alias  halPinH: pinH

    /*! \qmlproperty HalPin halPinS

        This property holds the s HAL pin providing the saturation compoent
        of the color.
    */
    property alias  halPinS: pinS

    /*! \qmlproperty HalPin halPinV

        This property holds the v HAL pin providing the value compoent
        of the color.
    */
    property alias  halPinV: pinV

    id: main

    HalPin {
        id: pinR
        name: main.name + ".r"
        type: HalPin.U32
        direction: HalPin.Out
    }
    HalPin {
        id: pinG
        name: main.name + ".g"
        type: HalPin.U32
        direction: HalPin.Out
    }
    HalPin {
        id: pinB
        name: main.name + ".b"
        type: HalPin.U32
        direction: HalPin.Out
    }
    HalPin {
        id: pinA
        name: main.name + ".a"
        type: HalPin.U32
        direction: HalPin.Out
    }
    HalPin {
        id: pinH
        name: main.name + ".h"
        type: HalPin.Float
        direction: HalPin.Out
    }
    HalPin {
        id: pinS
        name: main.name + ".s"
        type: HalPin.Float
        direction: HalPin.Out
    }
    HalPin {
        id: pinV
        name: main.name + ".v"
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
        visible: !(pinR.synced
                 && pinG.synced
                 && pinB.synced
                 && pinA.synced
                 && pinH.synced
                 && pinS.synced
                 && pinV.synced)
    }

    Binding { target: pinR; property: "value"; value: parseInt(main.colorValue.toString().substr(1, 2), 16)}
    Binding { target: pinG; property: "value"; value: parseInt(main.colorValue.toString().substr(3, 2), 16)}
    Binding { target: pinB; property: "value"; value: parseInt(main.colorValue.toString().substr(5, 2), 16)}
    Binding { target: pinA; property: "value"; value: Math.ceil(main.colorValue.a * 255)}
    Binding { target: pinH; property: "value"; value: main.colorValue.h}    //TODO
    Binding { target: pinS; property: "value"; value: main.colorValue.s}
    Binding { target: pinV; property: "value"; value: main.colorValue.v}
}
