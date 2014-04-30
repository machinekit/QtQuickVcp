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
        caption: qsTr("Progress Bar")

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
                text: qsTr("Indeterminate")
                }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.indeterminate
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Orientation")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.orientation
                    minimumValue: 0
                    maximumValue: 1
                    decimals: 0
                }
                ExpandingSpacer {
                }
            }
        }
    }
}
