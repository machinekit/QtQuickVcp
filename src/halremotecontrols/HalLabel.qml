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
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0

/*!
    \qmltype HalLabel
    \inqmlmodule Machinekit.HalRemote.Controls
    \brief A text label.
    \ingroup halremotecontrols

    This component provides a label combined with a HAL pin. By default
    the type of the HAL pin is float and the direction in.

    Use the \l{decimals}, \l{prefix} and \l {suffix} variables to format
    the output to your needs.

    \qml
    HalLabel {
        id: halLabel
        name: "label"
    }
    \endqml

    \sa Label
*/

Label {
    /*! This property holds the name of the default HAL pin.
    */
    property string name:       "label"

    /*! This property indicates the amount of decimals.
      Note that if you enter more decimals than specified, they will
      be truncated to the specified amount of decimal places.

      The default value is \c{2}.
    */
    property int    decimals:   2

    /*! The prefix for the value. I.e "$"
    */
    property string prefix:     ""

    /*! The suffix for the value. I.e "cm"
    */
    property string suffix:     ""

    /*! \qmlproperty HalPin halPin

        This property holds the default HAL pin.
    */
    property alias  halPin:     pin

    id: main

    text: main.prefix + (typeof pin.value === "number" ? pin.value.toFixed(main.decimals) : pin.value ) + main.suffix

    HalPin {
        id: pin

        name: main.name
        type: HalPin.Float
        direction: HalPin.In
    }
}
