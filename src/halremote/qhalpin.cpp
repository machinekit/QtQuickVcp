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
#include "qhalpin.h"

/*!
    \qmltype HalPin
    \instantiates QHalPin
    \inqmlmodule Machinekit.HalRemote
    \brief A HAL pin.
    \ingroup halremote

    This component provides a the counterpart of one pin of
    a HAL remote component. The HalPin component works in combination
    with the \l HalRemoteComponent.

    \qml
    HalPin {
        id: halPin
        name: "pin"
        type: HalPin.Float
        direction: HalPin.Out
    }
    \endqml

    Adding a HAL pin to a custom QML item can be done using using bindings.

    \note In order to visualize changes from other UI instances HAL pins
    with the \l direction \c Out must use a bidirectional binding.

    \qml
    import QtQuick.Components 1.0

    Button {
        id: button

        HalPin {
            id: pin

            name: main.name
            type: HalPin.Bit
            direction: HalPin.Out
        }

        Binding { target: button; property: "pressed"; value: pin.value}
        Binding { target: pin; property: "value"; value: button.pressed}
    }
    \endqml

    \sa HalRemoteComponent, HalButton
*/

/*! \qmlproperty string HalPin::name

    This property holds the name of the HAL pin.
*/

/*! \qmlproperty enumeration HalPin::type

    This property holds the type of the HAL pin.

    \list
    \li HalPin.Bit - The \c Bit type refers to a binary true/false value. (Default)
    \li HalPin.Float - The \c Float type refers to a number with decimal point, e.g. 1.2 or -29.8.
    \li HalPin.S32 - The \c S32 type referes to whole singed number, e.g. 0, 10, or -20.
    \li HalPin.U32 - The \c U32 type referes to whole unsinged number, e.g. 0, 10, or 32.
    \endlist
*/

/*! \qmlproperty enumeration HalPin::direction

    This property holds the direction of the HAL pin. The direction of the HAL
    pin must always be seen from the view of the HAL remote component containing
    the pin, e.g. a button usually has an output pin.

    \list
    \li HalPin.In - The \c In direction refers to a input pin. (Default)
    \li HalPin.Out - The \c Out direction refers to a output pin.
    \li HalPin.IO - The \c IO direction refers to a input/output pin.
    \endlist
*/

/*! \qmlproperty variant HalPin::value

    This property holds the value of the pin. The type of the value is
    evaluated using the \l type property.
*/

/*! \qmlproperty bool HalPin::enabled

    This property holds whether the HAL pin is enabled or not. This property
    is used by \l HalRemoteComponent to evaluate whether the pins should be
    included in the pin list or not. A disabled pin will be ignored by the
    \l{HalRemoteComponent}.

    The default value is \c{true}.
*/

/*! \qmlproperty bool HalPin::synced

    This property holds whether the pin is synced or not. The value is
    updated with the \l value property. When the \l value is set from
    QML the synced property changes to \c{false}. The property changes
    to \c true when the echo from the \l HalRemoteComponent is received.
*/

QHalPin::QHalPin(QObject *parent) :
    QObject(parent),
    m_name("default"),
    m_type(Bit),
    m_direction(Out),
    m_value(false),
    m_handle(0),
    m_enabled(true),
    m_synced(false)
{
}
