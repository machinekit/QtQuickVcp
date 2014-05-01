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
import Hal 1.0 as HAL
import Hal.Controls 1.0 as H

H.ApplicationWindow {
    property double commandValue: 0.0

    id: main

    width: 400
    height: 600
    viewPage: viewPage
    name: "pidctrl"
    title: qsTr("PID Control Demo")

    Item {
        id: viewPage

        anchors.fill: parent

        ColumnLayout {
            anchors.margins: 10
            anchors.fill: parent
            spacing: 0

            GroupBox {
                Layout.fillWidth: true
                Layout.preferredHeight: main.height * 0.3
                title: qsTr("Feedback Log")

                RowLayout {
                    anchors.fill: parent
                    spacing: 5

                    H.LogChart {
                        name: "feedback"

                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        targetValue: commandValue
                        minValue: minSlider.value
                        maxValue: maxSlider.value
                    }

                    ColumnLayout {
                        Layout.fillHeight: true

                        Text {
                            text: qsTr("Min")
                        }

                        Slider {
                            id: minSlider

                            orientation: Qt.Vertical
                            minimumValue: 0.0
                            maximumValue: 300.0
                            value: 0.0
                        }

                        Text {
                            text: minSlider.value.toFixed(2)
                        }
                    }
                    ColumnLayout {
                        Text {
                            text: qsTr("Max")
                        }

                        Slider {
                            id: maxSlider

                            Layout.fillHeight: true
                            orientation: Qt.Vertical
                            minimumValue: 0.0
                            maximumValue: 300.01
                            value: 300.0
                        }

                        Text {
                            text: maxSlider.value.toFixed(2)
                        }
                    }
                }
            }

            GroupBox {
                Layout.fillWidth: true
                Layout.preferredHeight: main.height * 0.3
                title: qsTr("Output Log")

                RowLayout {
                    anchors.fill: parent
                    spacing: 5

                    H.LogChart {
                        name: "output"

                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        minValue: minSlider2.value
                        maxValue: maxSlider2.value
                        yGrid: 0.1
                        rightTextVisible: false
                        signalColor: "lightblue"
                    }

                    ColumnLayout {
                        Layout.fillHeight: true

                        Text {
                            text: qsTr("Min")
                        }

                        Slider {
                            id: minSlider2

                            orientation: Qt.Vertical
                            minimumValue: -20.0
                            maximumValue: 20.0
                            value: -10.0
                        }

                        Text {
                            text: minSlider2.value.toFixed(2)
                        }
                    }
                    ColumnLayout {
                        Text {
                            text: qsTr("Max")
                        }

                        Slider {
                            id: maxSlider2

                            Layout.fillHeight: true
                            orientation: Qt.Vertical
                            minimumValue: -20.0
                            maximumValue: 20.0
                            value: 10.0
                        }

                        Text {
                            text: maxSlider2.value.toFixed(2)
                        }
                    }
                }
            }

            TabView {
                Layout.fillWidth: true
                Layout.preferredHeight: main.height * 0.3
                frameVisible: true

                Tab {
                    title: qsTr("Command")
                    active: true

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 5

                        GroupBox {
                            title: qsTr("Commanded value")
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            H.Slider {
                                id: commandSlider
                                name: "command"

                                anchors.fill: parent
                                minimumValue: 0
                                maximumValue: 300
                                value: 0

                                onValueChanged: main.commandValue = value
                            }

                        }

                        H.ToggleButton {
                            name: "enable"

                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            text: qsTr("Enable PID cotnrol")
                        }
                    }
                }

                Tab {
                    title: qsTr("PID")
                    active: true

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 5

                        GroupBox {
                            title: qsTr("P gain")
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            H.Slider {
                                name: "Pgain"
                                pinDirection: HAL.Pin.HAL_IO

                                anchors.fill: parent
                                minimumValue: 0
                                maximumValue: 1
                                value: 0.3
                            }
                        }

                        GroupBox {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            title: qsTr("I gain")

                            H.Slider {
                                name: "Igain"
                                pinDirection: HAL.Pin.HAL_IO

                                anchors.fill: parent
                                minimumValue: 0
                                maximumValue: 1
                                value: 0.0
                            }
                        }

                        GroupBox {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            title: qsTr("D gain")

                            H.Slider {
                                name: "Dgain"
                                pinDirection: HAL.Pin.HAL_IO

                                anchors.fill: parent
                                minimumValue: 0
                                maximumValue: 1
                                value: 0.0
                            }
                        }

                    }
                }

                Tab {
                    title: qsTr("Error")
                    active: true

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 5

                        GroupBox {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            title: qsTr("Maximum Error")

                            H.Slider {
                                name: "maxerrorI"
                                pinDirection: HAL.Pin.HAL_IO

                                anchors.fill: parent
                                minimumValue: 0
                                maximumValue: 10
                                value: 1.0
                            }
                        }
                        GroupBox {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            title: qsTr("Bias")

                            H.Slider {
                                name: "bias"
                                pinDirection: HAL.Pin.HAL_IO

                                anchors.fill: parent
                                minimumValue: 0.0
                                maximumValue: 1.0
                                value: 0.5
                            }
                        }
                    }
                }
            }
        }
    }
}
