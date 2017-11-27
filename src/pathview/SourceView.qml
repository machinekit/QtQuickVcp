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
import QtQuick.Window 2.0
import Machinekit.Application 1.0
import Machinekit.Application.Controls 1.0

Item {
    property alias model: object.gcodeProgramModel
    property color selectedColor: "lightblue"
    property color activeColor: "lightcoral"
    property color executedColor: "lightgreen"
    property alias font: dummyLabel.font

    id: root

    SystemPalette { id: systemPalette }

    QtObject {
        id: d
        readonly property int currentLine: appObject.status.synced ? appObject.status.motion.motionLine : 1
        readonly property bool taskIsInReliableState: appObject.status.synced ? (appObject.status.task.execState === ApplicationStatus.TaskWaitingForMotion
                                                                                 || appObject.status.task.execState === ApplicationStatus.TaskDone) : false

        onCurrentLineChanged: {
            if (!taskIsInReliableState) {
                return;
            }

            listView.positionViewAtIndex(currentLine, ListView.Center);
        }
    }

    ApplicationObject {
        id: appObject
    }

    PathViewObject {
        id: object
    }

    Rectangle {
        id: lineNumberBackground
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: dummyLabel.width * (Math.ceil(Math.log(listView.count) / Math.LN10) + 1)  // adapting width to line numbers
        color: systemPalette.window
    }

    Rectangle {
        id: normalBackground
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: lineNumberBackground.right
        anchors.right: parent.right
        color: systemPalette.light
    }

    Label {
        id: dummyLabel
        text: "0"
        visible: false
    }

    ScrollView {
        frameVisible: true
        anchors.fill: parent

        ListView {
            id: listView
            anchors.fill: parent
            model: object.gcodeProgramModel
            delegate: Item {
                readonly property bool lineActive: Boolean(active)

                anchors.left: parent ? parent.left : undefined
                anchors.right: parent ? parent.right : undefined
                height: dummyLabel.height

                Item {
                    id: lineNumberRect
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: lineNumberBackground.width

                    Label {
                        anchors.fill: parent
                        anchors.rightMargin: 5
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.NoWrap
                        elide: Text.ElideRight
                        maximumLineCount: 1
                        horizontalAlignment: Text.AlignRight
                        color: selected ? "white" : label.color
                        font: dummyLabel.font
                        text: String(lineNumber)
                    }
                }

                Rectangle {
                    color: selected ? root.selectedColor : (active ? root.activeColor : (executed ? root.executedColor : "transparent"))
                    anchors.left: lineNumberRect.right
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom


                    Label {
                        id: label
                        anchors.fill: parent
                        anchors.leftMargin: 5
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.NoWrap
                        elide: Text.ElideRight
                        maximumLineCount: 1
                        font: dummyLabel.font
                        text: String(gcode).trim()
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (!(mouse.modifiers & Qt.ControlModifier)) {
                            object.gcodeProgramModel.clearSelectionAndSelectLine(fileName, lineNumber);
                        }
                        else {
                            selected = !selected;
                        }
                        listView.currentIndex = index;
                    }
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            contextMenu.popup();
        }
    }

    Menu {
        id: contextMenu
        MenuItem {
            action: runProgramAction
            text: qsTr("Run Program from Line %1").arg(runProgramAction.programStartLine)
            enabled: runProgramAction.enabled && (listView.currentIndex > -1)
        }
        RunProgramAction {
            id: runProgramAction
            programStartLine: listView.currentIndex + 1
            shortcut: ""
        }
        MenuSeparator {}
        MenuItem { action: editAction }
        EditWithSystemEditorAction {
            id: editAction
        }
    }
}
