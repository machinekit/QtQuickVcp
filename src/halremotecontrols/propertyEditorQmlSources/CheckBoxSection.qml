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
    caption: qsTr("Check Box")

    SectionLayout {

        Label {
            text: qsTr("Text")
            tooltip:  qsTr("The text shown on the check box")
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
            tooltip: qsTr("The state of the check box")
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
            text: qsTr("Focus on press")
            tooltip: qsTr("Determines whether the check box gets focus if pressed.")
        }

        SecondColumnLayout {
            CheckBox {
                backendValue: backendValues.activeFocusOnPress
                implicitWidth: 180
            }
            ExpandingSpacer {

            }
        }

    }
}
