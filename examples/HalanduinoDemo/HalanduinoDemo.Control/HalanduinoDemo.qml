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
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import Machinekit.HalRemote.Controls 1.0
import Machinekit.HalRemote 1.0

HalApplicationWindow {
    id: main

    name: "halanduinoctrl"
    title: qsTr("Halanduino Demo")

    ScrollView  {
        id: scrollView1
        anchors.fill: parent

        RowLayout {
            id: rowLayout1
            width: main.width-20
            height: main.height*0.15
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10

            HalRoundGauge {
                id: roundGauge1
                name: "gauge1"
                x: 122
                y: 120
                Layout.fillHeight: true
            }

            HalRoundGauge {
                id: roundGauge2
                name: "gauge2"
                x: 228
                y: 120
                Layout.fillHeight: true
            }

            HalRoundGauge {
                id: roundGauge3
                name: "gauge3"
                x: 186
                y: 24
                Layout.fillHeight: true
            }

            Item {
                id: item1
                x: 370
                width: 200
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }

        /*ColumnLayout {
            property int margins: Screen.logicalPixelDensity*3
            width: main.width - margins*2
            x: margins
            y: margins
            spacing: 0

            GroupBox {
                Layout.fillWidth: true
                Layout.preferredHeight: main.height * 0.3
                title: qsTr("feedback")

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 5
                }
            }

        }*/
    }
}
