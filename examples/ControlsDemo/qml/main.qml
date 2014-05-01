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
import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.0
import Machinekit.Controls 1.0
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0
import Machinekit.VideoView 1.0
import Machinekit.PathView 1.0

ApplicationWindow {
    id: mainWindow

    visible: true
    title: qsTr("MachineKit Controls Demo")
    width: 500
    height: 700

    GroupBox {
        id: groupBox1
        x: 8
        y: 8
        width: 484
        height: 300
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        title: qsTr("Log Chart")

        HalLogChart {
            id: logChart1
            anchors.fill: parent
        }
    }

    GroupBox {
        id: groupBox3
        anchors.top: groupBox2.bottom
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        title: qsTr("Joystick")

        HalVirtualJoystick {
            id: virtualJoystick1
            width: height
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
        }

        HalVirtualJoystick {
            id: virtualJoystick2
            width: height
            autoCenter: false
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
        }

        HalLed {
            id: ledRound1
            x: 170
            y: 0
            value: true
        }

        HalLed {
            id: ledSquare1
            x: 268
            y: 0
            radius: 0
            value: true
            anchors.bottom: ledRound1.bottom
        }

        HalLed {
            id: ledSquare2
            x: 268
            y: 36
            radius: 0
            blink: true
            onColor: "#00cc08"
            value: true
            anchors.topMargin: 6
            anchors.top: ledRound1.bottom
            anchors.left: ledSquare1.left
            anchors.right: ledSquare1.right
        }

        HalLed {
            id: ledSquare3
            x: 268
            y: 72
            radius: 0
            onColor: "#f9d515"
            value: true
            anchors.topMargin: 6
            anchors.top: ledSquare2.bottom
            anchors.left: ledSquare1.left
            anchors.right: ledSquare1.right
        }

        HalLed {
            id: ledRound2
            x: 170
            y: 36
            blinkInterval: 1000
            blink: true
            onColor: "#01c015"
            value: true
            anchors.bottom: ledSquare2.bottom
            anchors.top: ledSquare2.top
            anchors.left: ledRound1.left
            anchors.right: ledRound1.right
        }

        HalLed {
            id: ledRound3
            x: 170
            y: 72
            radius: 10
            onColor: "#daed08"
            value: true
            anchors.bottom: ledSquare3.bottom
            anchors.top: ledSquare3.top
            anchors.left: ledRound1.left
            anchors.right: ledRound1.right
        }
    }

    GroupBox {
        id: groupBox2
        height: parent.height*0.3
        anchors.top: groupBox1.bottom
        anchors.topMargin: 6
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.leftMargin: 8
        title: qsTr("Controls")
        anchors.left: parent.left

        RowLayout {
            id: rowLayout1
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left

            ColumnLayout {
                id: columnLayout1
                width: 80
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0

                HalButton {
                    id: button1
                    x: 68
                    y: 78
                    text: qsTr("Button")
                    Layout.fillWidth: true
                    Layout.fillHeight: false
                }

                HalCheckBox {
                    id: checkBox1
                    x: 70
                    y: 28
                    text: qsTr("Check Box")
                    Layout.fillWidth: true
                }

                HalRadioButton {
                    id: radioButton1
                    text: qsTr("Radio Button")
                    Layout.fillWidth: true
                }

                Item {
                    id: filler
                    x: 258
                    y: 16
                    width: 200
                    height: 200
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }

}
