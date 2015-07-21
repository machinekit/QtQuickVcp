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
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0

/*!
    \qmltype HalComboBox
    \inqmlmodule Machinekit.HalRemote.Controls
    \brief Provides a drop-down list functionality.
    \ingroup halremotecontrols

    This component provides a combo box combined with a HAL pin. By default
    the type of the HAL pin is float and the direction out.

    There are two modes the combo box can be used in. In the index
    mode the HAL pin is set to the current index of the combo box.

    \qml
    HalComboBox {
        id: halComboBox
        name: "comboBox"
        mode: 0 // Index mode
        model: [ "Banana", "Apple", "Coconut" ]
    }
    \endqml

    In the model value mode the HAL pin is set to model value
    of the selected item of the combo box.

    \qml
    HalComboBox {
        id: halComboBox
        name: "comboBox"
        currentIndex: 0
        mode: 1 // Model value mode
        model: ListModel {
            ListElement { text: "Banana"; value: 1.0 }
            ListElement { text: "Apple"; value: 2.5 }
            ListElement { text: "Coconut"; value: 3.2 }
        }
    }
    \endqml

    \sa ComboBox
*/

ComboBox {
    /*! This property holds the name of the default HAL pin.
    */
    property string name:   "comboBox"

    /*! This property holds the output mode.

        \list
        \li 0 - Index mode (Default)
        \li 1 - Model value mode
        \endlist
    */
    property int    mode:   0 // Mode 0 == currentIndex, Mode 1 == model value

    /*! \qmlproperty HalPin halPin

        This property holds the default HAL pin.
    */
    property alias  halPin: pin

    id: main

    model: ListModel {
        ListElement { text: "Banana"; value: 1.0 }
        ListElement { text: "Apple"; value: 2.5 }
        ListElement { text: "Coconut"; value: 3.2 }
    }

    HalPin {
        id: pin

        name: main.name
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
        visible: !pin.synced
    }

    Binding { target: main; property: "currentIndex"; value: pin.value; when: (main.mode == 0)}
    Binding { target: pin; property: "value"; value: main.currentIndex; when: (main.mode == 0)}
    Binding { target: pin; property: "value"; value: (main.model.get === undefined) || main.model.get(main.currentIndex).value; when: (main.mode == 1)}
}
