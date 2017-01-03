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

import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Machinekit.Controls 1.0
import Machinekit.Application.Controls 1.0

ApplicationWindow {
    property alias model: object.gcodeProgramModel
    property color selectedColor: "lightblue"
    property color activeColor: "lightcoral"
    property color executedColor: "lightgreen"
    visible: false

    id: root

    SystemPalette { id: systemPalette }

    PathViewObject {
        id: object
        property var saveAsFile: false
        onGcodeEditModeChanged: {
            if ((!object.gcodeEditMode) &&
                ((textArea.text !== object.gcodeProgramLoader.text)
                 || (saveAsFile === true)))
            {
                // switching from edit-mode to read-mode
                console.log("TODO: confirm before saving")
                object.gcodeProgramLoader.save(textArea.text)
                object.core.file.startUpload()
                saveAsFile = false
            }
        }
    }

    Action {
        id: cutAction
        text: "Cut"
        shortcut: "ctrl+x"
        onTriggered: textArea.cut()
    }

    Action {
        id: copyAction
        text: "Copy"
        shortcut: "Ctrl+C"
        onTriggered: textArea.copy()
    }

    Action {
        id: pasteAction
        text: "Paste"
        shortcut: "ctrl+v"
        onTriggered: textArea.paste()
    }

    Action {
        id: fileSaveAsAction
        text: "Save As…"
        property bool editFileNameMode: false

        checkable: true
        checked: editFileNameMode

        // change file name
        onTriggered: {
            fileNameField.readOnly = editFileNameMode
            editFileNameMode = !editFileNameMode

            if (editFileNameMode === true) { return }

            var newFileName = fileNameField.text

            if (newFileName === object.core.file.fileName)
                return

            console.log("TODO: warning if remote file exists")
            //TODO: if (object.core.file.isFileExist(newFileName)) {
            //TODO:     console.log(newFileName + "exist")
            //TODO: }

            object.saveAsFile = true
            object.core.file.setFileName(newFileName);
            object.core.file.setLocalFilePath(object.core.file.localPath + '/' + newFileName);
            object.core.file.setRemoteFilePath(object.core.file.remotePath + '/' + newFileName);

            // console.log("saveAsFile: " + object.saveAsFile)
            // console.log("file.fileName: " + object.core.file.fileName)
            // console.log("file.localFilePath: " + object.core.file.localFilePath)
            // console.log("localFilePath: " + object.gcodeProgramLoader.localFilePath)
            // console.log("localPath: " + object.gcodeProgramLoader.localPath)
            // console.log("remotePath: " + object.gcodeProgramLoader.remotePath)
        }
    }

    Rectangle {
        color: systemPalette.dark
        id: mainToolBar
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.height * 0.125

        Flickable {
            flickableDirection: Flickable.VerticalFlick
            anchors.fill: parent

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: Screen.pixelDensity / 2

                TouchButton {
                    Layout.fillWidth: true
                    Layout.preferredHeight: width
                    action: fileSaveAsAction
                    iconSource: ""
                    FontIcon { // save
                        text: "\uE161"; color: systemPalette.light
                    }
                }

                TouchButton {
                    Layout.fillWidth: true
                    Layout.preferredHeight: width
                    action: copyAction
                    iconSource: ""
                    FontIcon { // content_copy
                        text: "\uE14D"; color: systemPalette.light
                    }
                }

                TouchButton {
                    Layout.fillWidth: true
                    Layout.preferredHeight: width
                    action: cutAction
                    iconSource: ""
                    FontIcon { // content_cut
                        text: "\uE14E"; color: systemPalette.light
                    }
                }

                TouchButton {
                    Layout.fillWidth: true
                    Layout.preferredHeight: width
                    action: pasteAction
                    iconSource: ""
                    FontIcon { // content_paste
                        text: "\uE14F"; color: systemPalette.light
                    }
                }

               TouchButton {
                    Layout.fillWidth: true
                    Layout.preferredHeight: width
                    action : EditProgramAction {
                        editor: root
                    }
                    iconSource: ""
                    FontIcon { // mode_edit
                        text: "\ue254"; color: systemPalette.light
                    }
                }

                Item { Layout.fillHeight: true }
            } // ColumnLayout
        } // Flickable
    } // Rectangle

    TextField {
        id: fileNameField
        anchors.top: parent.top
        anchors.left: mainToolBar.right
        anchors.right: parent.right
        anchors.bottom: textArea.top
        text: object.core.file.fileName
        readOnly: true
        textColor: systemPalette.light
        style: TextFieldStyle {
            background: Rectangle {
                color: {
                    if (fileSaveAsAction.editFileNameMode == true) systemPalette.highlight
                    else systemPalette.dark
                }
            }
        }
        // TODO: readonly
    }

    TextArea {
        id: textArea
        anchors.top: fileNameField.bottom
        anchors.left: mainToolBar.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        frameVisible: false
        textFormat: Qt.PlainText
        text: object.gcodeProgramLoader.text
        Component.onCompleted: forceActiveFocus()
        wrapMode: TextArea.Wrap
    }

}
