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

Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Button")

        SectionLayout {

            Label {
                text: qsTr("Text")
                tooltip:  qsTr("The text shown on the button")
            }

            SecondColumnLayout {
                LineEdit {
                    backendValue: backendValues.text
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Checked")
                tooltip: qsTr("The state of the button")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.checked
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Checkable")
                tooltip: qsTr("Determines whether the button is checkable or not.")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.checkable
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Enabled")
                tooltip: qsTr("Determines whether the button is enabled or not.")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.enabled
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }


            Label {
                text: qsTr("Default button")
                tooltip: qsTr("Sets the button as the default button in a dialog.")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.defaultbutton
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Tool tip")
                tooltip: qsTr("The tool tip shown for the button.")
            }

            SecondColumnLayout {
                LineEdit {
                    backendValue: backendValues.toolTip
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Focus on press")
                tooltip: "Determines whether the button gets focus if pressed."
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.activeFocusOnPress
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }


            Label {
                text: qsTr("Icon source")
                tooltip: qsTr("The URL of an icon resource.")
            }

            SecondColumnLayout {
                LineEdit {
                    backendValue: backendValues.iconSource
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }


        }
    }