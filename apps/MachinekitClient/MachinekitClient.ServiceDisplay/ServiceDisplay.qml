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

    /* Disconnects this page */
    signal disconnect()

    id: main

    color: systemPalette.window

    SystemPalette {
        id: systemPalette;
        colorGroup: enabled ? SystemPalette.Active : SystemPalette.Disabled
    }

    Label {
        id: dummyText
        visible: false
    }

    Button {
        id: dummyButton
        visible: false
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Screen.pixelDensity
        spacing: Screen.pixelDensity

        Label {
            id: pageTitleText

            Layout.fillWidth: true
            Layout.preferredHeight: Math.max(dummyButton.height, implicitHeight)
            text: qsTr("Discovered Services")
            font.pointSize: dummyText.font.pointSize * 1.3
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap

            RowLayout {
                anchors.fill: parent

                Item { Layout.fillWidth: true }
                Button {
                    text: qsTr("Back")
                    onClicked: main.disconnect()
                }
            }
        }

        ScrollView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                spacing: Screen.pixelDensity * 3

                model: allServices.items

                delegate: ColumnLayout {
                    property bool folded: true

                    id: serviceItem
                    anchors.leftMargin: Screen.pixelDensity * 3
                    anchors.rightMargin: Screen.pixelDensity * 3
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Label {
                        Layout.fillWidth: true
                        text: name
                        font.bold: true
                        MouseArea {
                            anchors.fill: parent
                            onClicked: serviceItem.folded ^= 1
                            cursorShape: Qt.PointingHandCursor
                        }
                    }
                    Label {
                        Layout.fillWidth: true
                        text: qsTr("host name = %1\n" +
                                   "host address = %2\n" +
                                   "port = %3").arg(hostName).arg(hostAddress).arg(port)
                        visible: !serviceItem.folded
                    }
                    Label {
                        Layout.fillWidth: true
                        text: txtRecords.join("\n")
                        visible: !serviceItem.folded
                    }
                }
            }
        }
    }
}
