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
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import Machinekit.Application 1.0
import Machinekit.Application.Controls 1.0

TableView {
    property alias core: object.core
    property alias history: object.mdiHistory

    signal commandSelected(string command)
    signal commandTriggered(string command)

    id: mdiTableView
    Layout.fillWidth: true
    Layout.fillHeight: true
    model: object.mdiHistory.model
    headerVisible: false

    onClicked: commandSelected(model[row].command)
    onDoubleClicked: commandTriggered(model[row].command)
    onRowCountChanged: mdiTableView.positionViewAtRow(rowCount-1, ListView.End)
    onPressAndHold: clearMenu.popup()

    Keys.onReturnPressed: {
        if (currentRow > -1) {
            commandTriggered(model[currentRow].command);
        }
    }

    ApplicationObject {
        id: object
    }

    TableViewColumn{ role: "command"  ; title: ""  }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            clearMenu.popup();
        }
    }

    Menu {
        id: clearMenu
        MenuItem { action: clearListAction }
        Action {
            id: clearListAction
            text: qsTr("Clear History")
            onTriggered: history.clear()
        }
    }
}
