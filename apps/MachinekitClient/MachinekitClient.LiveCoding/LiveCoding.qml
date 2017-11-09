/****************************************************************************
**
** Copyright (C) 2017 Alexander Rössler
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
** Alexander Rössler <alexander AT roessler DOT systems>
**
****************************************************************************/

import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Window 2.0
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import Machinekit.Application 1.0

Item {
    property string title: qsTr("QtQuickVcp Live Coding") + (loader.item ? " - " + loader.item.title : "")
    property var statusBar: null
    property var menuBar: null
    property var toolBar: null
    property var services: (((loader.item !== null) && (loader.item.services !== undefined)) ? loader.item.services : [])

    /* Disconnects this page */
    signal disconnect()

    id: root

    QtObject {
        id: d

        function reload() {
            loader.source = "";
            ApplicationHelpers.clearQmlComponentCache();
            loader.source = fileDialog.file;
        }

        function openWithSystemEditor() {
            ApplicationHelpers.openUrlWithDefaultApplication(fileDialog.file);
        }
    }

    Label {
        id: dummyText
        visible: false
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Button {
                text: qsTr("Open")
                onClicked: fileDialog.open()
            }

            Button {
                text: qsTr("Edit")
                onClicked: d.openWithSystemEditor()
            }

            Button {
                text: qsTr("Reload")
                onClicked: d.reload()
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                text: qsTr("Disconnect")
                onClicked: root.disconnect()
            }
        }

        Loader {
            id: loader
            Layout.fillWidth: true
            Layout.fillHeight: true

            onStatusChanged: {
                if (status !== Loader.Error) {
                    return;
                }

                var msg = loader.sourceComponent.errorString();
                errorLabel.text = qsTr("QML Error: Loading QML file failed:\n") + msg;
            }

            Label {
                id: errorLabel
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
                font.pointSize: dummyText.font.pointSize * 1.1
                visible: loader.status === Loader.Error
            }
        }
    }

    FileDialog {
        property bool selected: false
        id: fileDialog
        folder: StandardPaths.writableLocation(StandardPaths.HomeLocation)
        onFileChanged: d.reload()
        onAccepted: selected = true
    }

    FileWatcher {
        id: fileWatcher
        fileUrl: fileDialog.selected ? fileDialog.folder : ""
        recursive: true
        onFileChanged: d.reload()
    }
}
