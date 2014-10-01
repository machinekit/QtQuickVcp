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
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import Machinekit.Application 1.0

RowLayout {
    property alias text: mdiTextField.text
    property alias action: mdiCommandAction
    property alias core: mdiCommandAction.core
    property alias status: mdiCommandAction.status
    property alias command: mdiCommandAction.command
    property alias mdiHistory: mdiCommandAction.mdiHistory

    Layout.fillWidth: true
    Layout.fillHeight: false

    TextField {
        id: mdiTextField
        Layout.fillWidth: true
        Layout.fillHeight: true
        onAccepted: {
            mdiCommandAction.trigger()
        }
    }

    Button {
        Layout.fillHeight: true
        Layout.fillWidth: false
        action: mdiCommandAction
    }

    MdiCommandAction {
        id: mdiCommandAction
        mdiCommand: mdiTextField.text
    }
}
