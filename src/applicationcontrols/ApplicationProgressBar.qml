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
import Machinekit.Application 1.0

ProgressBar {
    property alias core: object.core
    property alias file: object.file
    property string _mode: getMode()

    id: progressBar
    value: getProgress()

    function getMode() {
        if ((file !== undefined) && (file.transferState === ApplicationFile.UploadRunning)) {
            return "upload"
        }
        else if ((file !== undefined) && (file.transferState === ApplicationFile.DownloadRunning)) {
            return "download"
        }
        else {
            return ""
        }
    }

    function getText() {
        if (_mode == "upload") {
            return qsTr("Uploading file ") + file.fileName
        }
        else if (_mode == "download") {
            return qsTr("Downloading file ") + file.fileName
        }
        else {
            return ""
        }
    }

    function getProgress() {
        if (_mode == "upload") {
            return file.progress
        }
        else if (_mode == "download") {
            return file.progress
        }
        else {
            return 0.0
        }
    }

    Label {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: cancelButton.left
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideRight
        text: parent.getText()
    }

    ToolButton {
        id: cancelButton
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        iconSource: "qrc:Machinekit/Application/Controls/icons/dialog-cancel"
        visible: _mode == "upload"
        onClicked: {
            parent.file.abort()
        }
    }

    ApplicationObject {
        id: object
    }
}
