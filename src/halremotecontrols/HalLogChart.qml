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
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0

/*!
    \qmltype HalLogChart
    \inqmlmodule Machinekit.HalRemote.Controls
    \brief A chart for logging data.
    \ingroup halremotecontrols

    This component provides a log chart combined with a HAL pin.
    By default the type of the HAL pin is float and the direction in.

    By default the log chart logs data on every change of the HAL pin

    \qml
    HalLogChart {
        id: halLogChart
        name: "logChart"
    }
    \endqml

    \sa LogChart
*/

LogChart {
    /*! This property holds the name of the default HAL pin.
    */
    property string name:   "logChart"

    /*! \qmlproperty HalPin halPin

        This property holds the default HAL pin.
    */
    property alias  halPin: pin

    id: main

    autoSampling: false

    HalPin {
        id: pin

        name: main.name
        type: HalPin.Float
        direction: HalPin.In

        onValueChanged: {
            if (!autoSampling)
                main.addData(value)
        }
    }

    Binding { target: main; property: "value"; value: pin.value}
}
