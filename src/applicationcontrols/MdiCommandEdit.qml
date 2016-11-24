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
    property int mdiHistoryPos: -1
    property alias button: mdiButton
    property alias textField: mdiTextField

    TextField {

        id: mdiTextField
        Layout.fillWidth: true

        placeholderText: qsTr("MDI")

        onAccepted: {
            if (text !== "") {
                mdiCommandAction.trigger();
            }
        }

        Keys.onUpPressed: {
            if (mdiHistory.model.length > 0) {
                if (mdiHistoryPos === -1) {
                    mdiHistoryPos = mdiHistory.model.length;
                }

                mdiHistoryPos -= 1;

                if (mdiHistoryPos === -1) {
                    mdiHistoryPos = 0;
                }

                mdiTextField.text = mdiHistory.model[mdiHistoryPos].command;
            }
        }

        Keys.onDownPressed: {
            if (mdiHistory.model.length > 0) {
                mdiHistoryPos += 1;

                if (mdiHistoryPos === mdiHistory.model.length) {
                    mdiHistoryPos -= 1;
                }

                mdiTextField.text = mdiHistory.model[mdiHistoryPos].command;
            }
        }
    }

    Button {
        id: mdiButton
        Layout.fillWidth: false
        action: mdiCommandAction
    }

    MdiCommandAction {
        id: mdiCommandAction
        mdiCommand: mdiTextField.text
        onTriggered: {
            mdiTextField.text = '';
            mdiHistoryPos = -1;
        }
    }
}
