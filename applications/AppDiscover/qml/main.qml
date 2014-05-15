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
import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Window 2.0
import Machinekit.HalRemote 1.0

ApplicationWindow {
    property var selectedInstance

    id: mainWindow

    visible: true
    title: (appLoader.active && (appLoader.item != null)) ? appLoader.item.title : qsTr("MachineKit App Discover")
    width: 500
    height: 700

    Text {
        id: dummyText
    }

    Item {
        id: pageStack

        anchors.fill: parent

        Item {
            id: discoveryPage

            anchors.fill: parent

            Text {
                id: pageTitleText2

                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: Screen.logicalPixelDensity*3
                text: qsTr("Available Instances:")
                font.pointSize: dummyText.font.pointSize * 1.6
                font.bold: true
            }

            ListView {
                anchors.top: pageTitleText2.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: Screen.logicalPixelDensity*3
                spacing: Screen.logicalPixelDensity*3

                model: appDiscovery.discoveredApps
                delegate: Button {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: mainWindow.height*0.1

                    Text {
                        id: titleText2

                        anchors.centerIn: parent
                        font.pointSize: dummyText.font.pointSize*1.6
                        font.bold: true
                        text: name
                    }

                    onClicked: {
                        if (appDiscovery.discoveredApps[index].dsname !== "")
                        {
                            selectedInstance = appDiscovery.discoveredApps[index]
                            appConfig.ready = true
                        }
                    }
                }
            }
        }

        Item {
            id: appPage

            anchors.fill: parent

            Text {
                id: pageTitleText

                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: Screen.logicalPixelDensity*3
                text: selectedInstance !== undefined ? selectedInstance.name : ""
                font.pointSize: dummyText.font.pointSize * 1.6
                font.bold: true
            }

            ListView {
                anchors.top: pageTitleText.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: Screen.logicalPixelDensity*3
                spacing: Screen.logicalPixelDensity*3

                model: appConfig.appConfigs
                delegate: Button {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: mainWindow.height*0.1

                    Text {
                        id: titleText

                        anchors.centerIn: parent
                        anchors.verticalCenterOffset: -Screen.logicalPixelDensity*2
                        font.pointSize: descriptionText.font.pointSize*1.6
                        font.bold: true
                        text: name
                    }
                    Text {
                        id: descriptionText

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: titleText.bottom
                        anchors.margins: Screen.logicalPixelDensity*1
                        text: description
                    }

                    onClicked: appConfig.selectAppConfig(name)
                }
            }
        }

        Item {
            id: viewPage

            anchors.fill: parent

            Loader {
                id: appLoader

                anchors.fill: parent
                active: appConfig.selectedConfig.loaded
                source: appConfig.selectedConfig.mainFile

                onSourceChanged: {
                    console.log("Source changed: " + source + " " + active)
                }

                onLoaded: {
                    console.log("Window " + appLoader.item.name + " loaded")
                    if (appConfig.services !== undefined)
                    {
                        appConfig.services = Qt.binding(function(){return (appLoader.item != null ? appLoader.item.services : [])})
                        appConfig.updateServices()
                    }
                }
            }
        }

        state: (appConfig.ready) ? ((appLoader.active) ?"loaded":"config") :"discovery"

        states: [
            State {
                name: "discovery"
                PropertyChanges { target: discoveryPage; opacity: 1.0; z: 1; enabled: true }
                PropertyChanges { target: appPage; opacity: 0.0; z: 0; enabled: false }
                PropertyChanges { target: viewPage; opacity: 0.0; z: 0; enabled: false }
            },
            State {
                name: "config"
                PropertyChanges { target: appPage; opacity: 1.0; z: 1; enabled: true }
                PropertyChanges { target: viewPage; opacity: 0.0; z: 0; enabled: false }
                PropertyChanges { target: discoveryPage; opacity: 0.0; z: 0; enabled: false }
            },
            State {
                name: "loaded"
                PropertyChanges { target: appPage; opacity: 0.0; z: 0; enabled: false }
                PropertyChanges { target: viewPage; opacity: 1.0; z: 1; enabled: true }
                PropertyChanges { target: discoveryPage; opacity: 0.0; z: 0; enabled: false }
            }
        ]

        transitions: Transition {
                PropertyAnimation { duration: 500; properties: "opacity"; easing.type: Easing.InCubic }
            }

        // Capture the Android Back key and backspace key
        // on the desktop tp go back in the application
        // focus needs to be true to capture key events
        focus: true
        Keys.onReleased: {
            if ((event.key === Qt.Key_Back) ||
                    (event.key === Qt.Key_Backspace)) {
                if (pageStack.state == "discovery")
                {
                    Qt.quit()
                }
                else if (pageStack.state == "config")
                {
                    appConfig.ready = false
                }
                else if (pageStack.state == "loaded")
                {
                    appConfig.unselectAppConfig()
                }

                event.accepted = true
            }
        }
    }

    AppConfig {
        id: appConfig

        uri: selectedInstance !== undefined ? selectedInstance.dsname : ""
        timeout: 3000
        ready: false
        filters: [ AppConfigFilter { type: AppConfigItem.QT5_QML },
                   AppConfigFilter { type: AppConfigItem.GLADEVCP } ]
    }

    AppDiscovery {
        id: appDiscovery
        regType: "_machinekit._tcp.local"
        running: pageStack.state == "discovery"
    }
}
