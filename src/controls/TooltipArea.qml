/****************************************************************************
**
** Copyright (C) 2016 Alexander Rössler
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
** Alexander Rössler @ The Cool Tool GmbH <mail AT roessler DOT systems>
**
****************************************************************************/

import QtQuick 2.3
import QtQuick.Controls.Private 1.0

// This file contains private Qt Quick modules that might change in future versions of Qt
// Tested with Qt 5.6.0

MouseArea {
    property string text: ""
    property int interval: 1000

    id: _root

    anchors.fill: parent
    hoverEnabled: _root.enabled

    onExited: Tooltip.hideText()
    onCanceled: Tooltip.hideText()

    Timer {
        interval: _root.interval
        running: _root.enabled && _root.containsMouse && _root.text.length
        onTriggered: {
            Tooltip.hideText();
            Tooltip.showText(_root, Qt.point(_root.mouseX, _root.mouseY), _root.text);
        }
    }
}
