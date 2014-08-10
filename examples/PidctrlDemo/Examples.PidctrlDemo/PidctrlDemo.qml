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
import Machinekit.Controls 1.0
import Machinekit.HalRemote 1.0
import Machinekit.HalRemote.Controls 1.0

HalApplicationWindow {
    property double commandValue: 0.0

    id: main

    width: 800
    height: 600
    name: "pidctrl"
    title: qsTr("PID Control Demo")

    ScrollView  {
        anchors.fill: parent

        ColumnLayout {
            property int margins: Screen.logicalPixelDensity*3
            width: main.width - margins*2
            x: margins
            y: margins
            spacing: 0

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: main.height * 0.5

                GroupBox {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    title: qsTr("Feedback Log")

                    RowLayout {
                        anchors.fill: parent
                        spacing: 5

                        HalLogChart {
                            id: feedbackLogChart
                            name: "feedback"

                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            targetValue: commandValue
                            minimumValue: minSlider.value
                            maximumValue: maxSlider.value
                            autoSampling: true
                            sampleInterval: 300
                            timeSpan: 60000*3 // 3min
                            xGrid: 10000 // 10s
                            yGrid: 10 //10°C
                        }

                        ColumnLayout {
                            Layout.fillHeight: true

                            Text {
                                text: qsTr("Min")
                            }

                            Slider {
                                id: minSlider

                                Layout.fillHeight: true
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
                    Layout.fillHeight: true
                    title: qsTr("Output Log")

                    RowLayout {
                        anchors.fill: parent
                        spacing: 5

                        HalLogChart {
                            id: outputLogChart
                            name: "output"

                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            minimumValue: minSlider2.value
                            maximumValue: maxSlider2.value
                            xGrid: feedbackLogChart.xGrid
                            yGrid: 0.5
                            rightTextVisible: false
                            signalColor: "lightblue"
                            autoSampling: true
                            sampleInterval: feedbackLogChart.sampleInterval

                            Binding { target: outputLogChart; property: "timeSpan"; value: feedbackLogChart.timeSpan }
                            Binding { target: feedbackLogChart; property: "timeSpan"; value: outputLogChart.timeSpan }

                        }

                        ColumnLayout {
                            Layout.fillHeight: true

                            Text {
                                text: qsTr("Min")
                            }

                            Slider {
                                id: minSlider2

                                Layout.fillHeight: true
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

              }
            RowLayout {
                Layout.fillWidth: true

                GroupBox {
                    Layout.fillWidth: true
                    Layout.preferredHeight: main.height * 0.3
                    title: qsTr("Command")

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 5

                        GroupBox {
                            title: qsTr("Commanded value")
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            HalSlider {
                                id: commandSlider
                                name: "command"

                                anchors.fill: parent
                                minimumValue: 0
                                maximumValue: 300
                                value: 0

                                onValueChanged: main.commandValue = value
                            }

                        }

                        HalButton {
                            name: "enable"

                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            checkable: true
                            text: qsTr("Enable PID cotnrol")
                        }
                    }
                }

                GroupBox {
                    Layout.fillWidth: true
                    Layout.preferredHeight: main.height * 0.3
                    title: qsTr("PID")

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 5

                        GroupBox {
                            title: qsTr("P gain")
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            HalSlider {
                                name: "Pgain"
                                halPin.direction: HalPin.IO

                                anchors.fill: parent
                                minimumValue: 0
                                maximumValue: 1
                                value: 0.3
                                stepSize: 0.001
                                decimals: 3
                            }
                        }

                        GroupBox {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            title: qsTr("I gain")

                            HalSlider {
                                name: "Igain"
                                halPin.direction: HalPin.IO

                                anchors.fill: parent
                                minimumValue: 0
                                maximumValue: 0.01
                                value: 0.0
                                stepSize: 0.00001
                                decimals: 5
                            }
                        }

                        GroupBox {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            title: qsTr("D gain")

                            HalSlider {
                                name: "Dgain"
                                halPin.direction: HalPin.IO

                                anchors.fill: parent
                                minimumValue: 0
                                maximumValue: 0.1
                                value: 0.0
                                stepSize: 0.0001
                                decimals: 4
                            }
                        }

                    }
                }

                GroupBox {
                    Layout.fillWidth: true
                    Layout.preferredHeight: main.height * 0.3
                    title: qsTr("Error")

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 5

                        GroupBox {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            title: qsTr("Maximum Error")

                            HalSlider {
                                name: "maxerrorI"
                                halPin.direction: HalPin.IO

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

                            HalSlider {
                                name: "bias"
                                halPin.direction: HalPin.IO

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
