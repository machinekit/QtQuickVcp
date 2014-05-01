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
import Hal.Controls 1.0 as H
import Hal 1.0 as HAL

H.ApplicationWindow {

    id: main

    width: 400
    height: 600
    viewPage: viewPage
    name: "motorctrl"
    title: qsTr("Motor Control Demo")

    Item  {
        id: viewPage
        anchors.fill: parent

        ColumnLayout {
            anchors.margins: 10
            anchors.fill: parent
            spacing: 0

            GroupBox {
                Layout.fillWidth: true
                Layout.preferredHeight: viewPage.height * 0.3
                title: qsTr("feedback")

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 5

                    GroupBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        title: qsTr("position")

                        H.ProgressBar {
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

                        H.Gauge {
                            name: "fb_vel"

                            anchors.fill: parent
                            minimumValue: -100
                            maximumValue: 100
                        }
                    }
                    GroupBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        title: qsTr("acceleration")

                        H.Gauge {
                            name: "fb_acc"

                            anchors.fill: parent
                            minimumValue: -100
                            maximumValue: 100
                        }
                    }
                }
            }

            GroupBox {
                Layout.fillWidth: true
                Layout.preferredHeight: viewPage.height * 0.5
                title: qsTr("control")

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 5

                    GroupBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        title: qsTr("commanded position")

                        H.Slider {
                            name: "cmd_pos"

                            anchors.fill: parent
                            minimumValue: 0
                            maximumValue: 100
                            value: 0
                        }
                    }
                    GroupBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        title: qsTr("maximum velocity")

                        H.Slider {
                            name: "cmd_maxvel"

                            anchors.fill: parent
                            minimumValue: 0
                            maximumValue: 100
                            value: 20
                        }
                    }
                    GroupBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        title: qsTr("maximum acceleration")

                        H.Slider {
                            name: "cmd_maxacc"

                            anchors.fill: parent
                            minimumValue: 0
                            maximumValue: 100
                            value: 10
                        }
                    }
                    GroupBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        title: qsTr("lowpass filter gain")

                        H.Slider {
                            name: "lowpass_gain"

                            anchors.fill: parent
                            minimumValue: 0
                            maximumValue: 0.9
                            value: 0.01
                        }
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

                        H.ToggleButton {
                            name: "togglebutton1"

                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            text: qsTr("togglebutton")

                            HAL.Pin {
                                name: "togglebutton1-not"
                                type: HAL.Pin.HAL_BIT
                                direction: HAL.Pin.HAL_OUT
                                value: !parent.value
                            }
                        }
                        H.ToggleButton {
                            name: "scope_trigger"

                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            text: qsTr("trigger scope")
                        }
                    }

                    ColumnLayout {
                        Layout.preferredWidth: main.width *0.4
                        Layout.fillHeight: true
                        spacing: 5

                        H.Led {
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
