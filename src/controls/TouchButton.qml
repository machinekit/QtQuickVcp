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
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import Machinekit.Controls 1.0

Button {
    id: button

    property string iconSource

    /*style: ButtonStyle {
            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 25
                border.width: 0 //control.activeFocus ? 2 : 1
                border.color: "#888"
                radius: 4
                gradient: Gradient {
                    GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#00000000" }
                    GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#00000000" }
                }
            }
        }*/
    Image {
               source: iconSource
               anchors.verticalCenter: parent.verticalCenter
               fillMode: Image.Stretch //mm Image should shrink if button is too small, depends on QTBUG-14957
               anchors.fill: parent
               anchors.margins: 8
    }
}
