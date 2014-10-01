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
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

RowLayout {
    property alias core: object.core
    property alias status: object.status
    property int axis: axisGroup.currentIndex
    property var axisNames: ["X", "Y", "Z", "A", "B", "C", "U", "V", "W"]

    id: root

    ExclusiveGroup {
        property int currentIndex: 0
        id: axisGroup
    }

    Repeater {
        model: status.synced ? status.config.axes : 0
        RadioButton {
            exclusiveGroup: axisGroup
            text: root.axisNames[index]
            checked: index == 0
            onCheckedChanged: {
                if (checked) {
                    axisGroup.currentIndex = index
                }
            }
        }
    }

    ApplicationObject {
        id: object
    }
}
