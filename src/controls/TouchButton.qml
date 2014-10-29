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
import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2

ToolButton {
    style: ButtonStyle {
        property bool hovered : (Qt.platform.os == "android") ? false : control.hovered
            background: Rectangle {
                border.color: (control.pressed || control.checked || hovered) ? systemPalette.shadow : "#00000000"
                border.width: hovered ? 2 : 1
                radius: height * 0.08
                color: (control.pressed || control.checked) ? systemPalette.highlight : (hovered) ? systemPalette.button : "#00000000"
            }
            label: Image {
                source: control.iconSource
                opacity: control.enabled ? 1.0 : 0.3
                smooth: true
                sourceSize: Qt.size(control.width, control.height)
            }
        }

    Layout.preferredWidth: height

    SystemPalette { id: systemPalette }
}
