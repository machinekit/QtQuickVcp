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
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Gauge")

        SectionLayout {
            Label {
                text: qsTr("Value")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.value
                    minimumValue: backendValues.minimumValue
                    maximumValue: backendValues.maximumValue
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Minimum value")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.minimumValue
                    minimumValue: -999999999999
                    maximumValue: backendValues.maximumValue
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Maximum value")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.maximumValue
                    minimumValue: backendValues.minimumValue
                    maximumValue: 999999999999
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Zero value")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.zeroValue
                    minimumValue: backendValues.minimumValue
                    maximumValue: backendValues.maximumValue
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("z0 border value")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.z0BorderValue
                    minimumValue: backendValues.minimumValue
                    maximumValue: backendValues.maximumValue
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("z1 border value")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.z1BorderValue
                    minimumValue: backendValues.minimumValue
                    maximumValue: backendValues.maximumValue
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }
            
            Label {
                text: qsTr("Step size")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.stepSize
                    minimumValue: 0.01
                    maximumValue: 999999999999
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }
            
            Label {
                text: qsTr("Visibility")
                toolTip: qsTr("Determines whether the items are visible or not.")
            }

            SecondColumnLayout {
                ColumnLayout {
                    CheckBox {
                        text: qsTr("Tickmarks")
                        backendValue: backendValues.tickmarksEnabled
                    }
                    CheckBox {
                        text: qsTr("Indicator")
                        backendValue: backendValues.indicatorVisible
                    }
                    CheckBox {
                        text: qsTr("Visualizer")
                        backendValue: backendValues.visualizerVisible
                    }
                    CheckBox {
                        text: qsTr("Needle")
                        backendValue: backendValues.needleVisible
                    }
                }

                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Invert")
                toolTip: qsTr("Determines whether the gauge is inverted or not.")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.invert
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Fancy")
                toolTip: qsTr("Determines whether the bar should be styled fancy or not.")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.fancy
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }
            
            Label {
                text: qsTr("Animated")
                toolTip: qsTr("Determines whether the gauge is animated or not.")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.animated
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }
            
            Label {
                text: qsTr("Start angle")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.startAngle
                    minimumValue: 0.0
                    maximumValue: 1.0
                    decimals: 3
                }
                ExpandingSpacer {
                }
            }
            
            Label {
                text: qsTr("End angle")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.endAngle
                    minimumValue: 0.0
                    maximumValue: 1.0
                    decimals: 3
                }
                ExpandingSpacer {
                }
            }
        }
    }

    SmallTextSection {
    }
    
    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Needle color")

        ColorEditor {
                    caption: qsTr("Needle color")
                    backendendValue: backendValues.needleColor
                    supportGradient: false
                }
    }
    
    Section {
            anchors.left: parent.left
            anchors.right: parent.right
            caption: qsTr("z0 color")

            ColorEditor {
                        caption: qsTr("z0 color")
                        backendendValue: backendValues.z0Color
                        supportGradient: false
                    }
    }

    Section {
            anchors.left: parent.left
            anchors.right: parent.right
            caption: qsTr("z1 color")

            ColorEditor {
                        caption: qsTr("z1 color")
                        backendendValue: backendValues.z1Color
                        supportGradient: false
                    }
    }

    Section {
            anchors.left: parent.left
            anchors.right: parent.right
            caption: qsTr("z2 color")

            ColorEditor {
                        caption: qsTr("z2 color")
                        backendendValue: backendValues.z2Color
                        supportGradient: false
                    }
    }

    Section {
            anchors.left: parent.left
            anchors.right: parent.right
            caption: qsTr("Text color")

            ColorEditor {
                        caption: qsTr("Text color")
                        backendendValue: backendValues.textColor
                        supportGradient: false
                    }
    }

    Section {
            anchors.left: parent.left
            anchors.right: parent.right
            caption: qsTr("Background color")

            ColorEditor {
                        caption: qsTr("Background color")
                        backendendValue: backendValues.backgroundColor
                        supportGradient: false
                    }
    }
    
    Section {
            anchors.left: parent.left
            anchors.right: parent.right
            caption: qsTr("Inner circling color")

            ColorEditor {
                        caption: qsTr("Inner circling color")
                        backendendValue: backendValues.innerCirclingColor
                        supportGradient: false
                    }
    }
    
    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Outer circling color")

        ColorEditor {
                    caption: qsTr("Outer circling color")
                    backendendValue: backendValues.outerCirclingColor
                    supportGradient: false
                }
    }
    
    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Graduation color")

        ColorEditor {
                    caption: qsTr("Graduation color")
                    backendendValue: backendValues.graduationColor
                    supportGradient: false
                }
    }
}
