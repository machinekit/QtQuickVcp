/****************************************************************************
**
** Copyright (C) 2015 Alexander Rössler
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
** Alexander Rössler <mail AT roessler DOT systems>
**
****************************************************************************/
import QtQuick 2.0
import Machinekit.HalRemote.Controls 1.0

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
