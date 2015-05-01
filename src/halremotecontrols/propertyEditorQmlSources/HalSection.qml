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
    caption: qsTr("Hal")

    SectionLayout {

        Label {
            text: qsTr("Name")
            tooltip: qsTr("Name of the main HAL pin")
        }

        SecondColumnLayout {
            LineEdit {
                backendValue: backendValues.name
                implicitWidth: 180
            }
            ExpandingSpacer {

            }
        }

        /*Label {
            text: qsTr("Type")
        }

        SecondColumnLayout {
            ComboBox {
                model: ["HAL_FLOAT", "HAL_S32", "HAL_U32", "HAL_BIT"]
                backendValue: backendValues.halPin_type
                implicitWidth: 180
                Layout.fillWidth: true
                scope: "Pin"
            }

            ExpandingSpacer {

            }
        }

        Label {
            text: qsTr("Direction")
        }

        SecondColumnLayout {
            ComboBox {
                model: ["HAL_IN", "HAL_OUT", "HAL_IO"]
                backendValue: backendValues.halPin_direction
                implicitWidth: 180
                Layout.fillWidth: true
                scope: "Pin"
            }

            ExpandingSpacer {

            }
        }*/

    }
}
