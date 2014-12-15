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
import Machinekit.Service 1.0

Rectangle {
    property string title: qsTr("Service Display")
    property var statusBar: null
    property var menuBar: null
    property var toolBar: null

    property list<Service> services: [
        Service {
            id: allServices
            type: ""
        }
    ]

    id: main

    color: systemPalette.window

    SystemPalette {
        id: systemPalette;
        colorGroup: enabled ? SystemPalette.Active : SystemPalette.Disabled
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Screen.logicalPixelDensity*3
        spacing: Screen.logicalPixelDensity*3

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: Screen.logicalPixelDensity*3

            model: allServices.items

            delegate: ColumnLayout {
                anchors.left: parent.left
                anchors.right: parent.right

                Label {
                    Layout.fillWidth: true
                    text: name
                    font.bold: true
                }
                Label {
                    Layout.fillWidth: true
                    text: txtRecords.join("\n")
                }
            }
        }
    }
}
