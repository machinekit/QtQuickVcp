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

    name: "motorctrl"
    title: qsTr("Motor Control Demo")

    ScrollView  {
        anchors.fill: parent

        ColumnLayout {
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

                    GroupBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        title: qsTr("position")

                        HalProgressBar {
                            name: "fb_pos"

                            anchors.fill: parent
                            minimumValue: 0
                            maximumValue: 100
                        }
                    }
                    GroupBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        title: qsTr("velocity")

                        HalGauge {
                            name: "fb_vel"

                            anchors.fill: parent
                            minimumValue: -100
                            maximumValue: 100
                            zeroValue: 0
                            fancy: false
                        }

                        HalPin {
                            name: "fb_vel.scale"
                            type: HalPin.Float
                            direction: HalPin.In
                        }
                    }
                    GroupBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        title: qsTr("acceleration")

                        HalGauge {
                            name: "fb_acc"

                            anchors.fill: parent
                            minimumValue: -100
                            maximumValue: 100
                            zeroValue: 0
                            fancy: false
                        }

                        HalPin {
                            name: "fb_acc.scale"
                            type: HalPin.Float
                            direction: HalPin.In
                        }
                    }
                }
            }

            GroupBox {
                Layout.fillWidth: true
                Layout.preferredHeight: main.height * 0.5
                title: qsTr("control")

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 5

                    Label {
                        Layout.fillWidth: true
                        text: qsTr("commanded position")
                    }
                    HalSlider {
                        name: "cmd_pos"

                        Layout.fillWidth: true
                        minimumValue: 0
                        maximumValue: 100
                        value: 0
                        tickmarksEnabled: false
                    }
                    Label {
                        Layout.fillWidth: true
                        text: qsTr("maximum velocity")
                    }
                    HalSlider {
                        name: "cmd_maxvel"

                        Layout.fillWidth: true
                        minimumValue: 0
                        maximumValue: 100
                        value: 20
                        tickmarksEnabled: false
                        textInverted: true
                    }
                    Label {
                        Layout.fillWidth: true
                        text: qsTr("maximum acceleration")
                    }
                    HalSlider {
                        name: "cmd_maxacc"

                        Layout.fillWidth: true
                        minimumValue: 0
                        maximumValue: 100
                        value: 10
                        tickmarksEnabled: false
                        maximumValueVisible: false
                        minimumValueVisible: false
                    }
                    Label {
                        Layout.fillWidth: true
                        text: qsTr("lowpass filter gain")
                    }
                    HalSlider {
                        name: "lowpass_gain"

                        Layout.fillWidth: true
                        minimumValue: 0
                        maximumValue: 0.9
                        value: 0.01
                        tickmarksEnabled: false
                    }
                    Item {
                        Layout.fillHeight: true
                    }
                }
            }
            GroupBox {
                Layout.fillWidth: true
                Layout.fillHeight: true
                title: qsTr("misc")

                RowLayout {
                    id: row1
                    anchors.fill: parent
                    spacing: 5

                    ColumnLayout {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        //Layout.: 2
                        spacing: 5

                        HalButton {
                            name: "togglebutton1"

                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            text: qsTr("togglebutton")
                            checkable: true

                            HalPin {
                                name: "togglebutton1-not"
                                type: HalPin.Bit
                                direction: HalPin.Out
                                value: !parent.value
                            }
                        }
                        HalButton {
                            name: "scope_trigger"

                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            text: qsTr("trigger scope")
                            checkable: true
                        }
                    }

                    ColumnLayout {
                        Layout.preferredWidth: main.width *0.4
                        Layout.fillHeight: true
                        spacing: 5

                        HalLed {
                            name: "led1"

                            x: parent.width * 0.4
                            y: parent.height * 0.3
                            width: parent.width * 0.15
                            height: width
                            blink: false
                        }
                    }
                }
            }
        }
    }
}
