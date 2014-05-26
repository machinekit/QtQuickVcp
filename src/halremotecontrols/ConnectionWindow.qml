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
import QtQuick.Controls 1.1
import QtQuick.Window 2.0
import Machinekit.HalRemote 1.0

Rectangle {
    property bool autoSelectInstance: false
    property alias instanceFilter: configService.filter
    property string applicationSource: ""

    readonly property string title: (applicationLoader.active && (applicationLoader.item != null))
                           ? ((applicationLoader.item.title !== undefined) ? applicationLoader.item.title : "")
                           : qsTr("MachineKit App Discover")
    readonly property bool localMode: applicationSource != ""

    id: mainWindow


    color: systemPalette.window
    width: 500
    height: 700



    function selectInstance(index)
    {
        if (!localMode)
        {
            if ((configService.serviceDiscoveryItems[index].uri !== "") && (configService.serviceDiscoveryItems[index].uuid !== ""))
            {
                var x = applicationConfig   // for some reason the appConfig variable goes away after settings the filter
                serviceDiscoveryFilter.txtRecords = ["uuid=" + configService.serviceDiscoveryItems[index].uuid]
                serviceDiscovery.updateFilter()
                x.ready = true
                discoveryPage.instanceSelected = true
            }
            else
            {
                console.log("selecting instance failed: check uri and uuid")
            }
        }
        else
        {
            if (configService.serviceDiscoveryItems[index].uuid !== "")
            {
                serviceDiscoveryFilter.txtRecords = ["uuid=" + configService.serviceDiscoveryItems[index].uuid]
                serviceDiscovery.updateFilter()
                discoveryPage.instanceSelected = true
            }
            else
            {
                console.log("selecting instance failed: check uuid")
            }
        }
    }

    function goBack()
    {
        if (pageStack.state == "discovery")
        {
            Qt.quit()
        }
        else if (pageStack.state == "config")
        {
            if (autoSelectInstance == false)
            {
                applicationConfig.ready = false
                serviceDiscoveryFilter.txtRecords = []
                serviceDiscovery.updateFilter()
            }
            else
            {
                Qt.quit()
            }
        }
        else if (pageStack.state == "loaded")
        {
            if (localMode)
            {
                if (autoSelectInstance == false)
                {
                    applicationServiceList.services = []
                    applicationInternalServiceList.services = []
                    serviceDiscovery.updateServices()
                    serviceDiscoveryFilter.txtRecords = []
                    serviceDiscovery.updateFilter()
                    discoveryPage.instanceSelected = false
                }
                else
                {
                    Qt.quit()
                }
            }
            else
            {
                applicationConfig.unselectApplicationConfig()
                applicationServiceList.services = []
                applicationInternalServiceList.services = []
                serviceDiscovery.updateServices()
            }
        }
    }

    SystemPalette {
        id: systemPalette;
        colorGroup: enabled ? SystemPalette.Active : SystemPalette.Disabled
    }

    Text {
        id: dummyText
    }

    Item {
        id: pageStack

        anchors.fill: parent

        Item {
            id: networkPage

            anchors.fill: parent

            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: Screen.logicalPixelDensity * 2
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                font.pixelSize: dummyText.font.pixelSize * 1.5
                text: qsTr("Warning!<br>No network connection found, service discovery unavailable. Please check your network connection.")
            }
        }

        Item {
            property bool instanceSelected: false

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

                model: configService.serviceDiscoveryItems
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

                    onClicked: selectInstance(index)
                }

                onCountChanged: {
                    if ((pageStack.state == "discovery") && (autoSelectInstance == true) && (count > 0))
                    {
                        selectInstance(0)
                    }
                }
            }

            BusyIndicator {
                anchors.centerIn: parent
                running: true
                visible: configService.serviceDiscoveryItems.length === 0
                height: parent.height * 0.15
                width: height
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
                text: configService.name
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

                model: applicationConfig.appConfigs
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

                    onClicked: applicationConfig.selectApplicationConfig(name)
                }
            }
        }

        Item {
            id: viewPage

            anchors.fill: parent

            Loader {
                id: applicationLoader

                anchors.fill: parent
                active: localMode ? discoveryPage.instanceSelected : applicationConfig.selectedConfig.loaded
                source: localMode ? applicationSource : applicationConfig.selectedConfig.mainFile

                onSourceChanged: {
                    console.log("Source changed: " + source + " " + active)
                }

                onLoaded: {
                    console.log("Window " + applicationLoader.item.name + " loaded")
                    applicationServiceList.services = Qt.binding(
                                function()
                                {
                                    return (((applicationLoader.item != null) && (applicationLoader.item.services !== undefined)) ? applicationLoader.item.services : [])
                                })
                    applicationInternalServiceList.services = Qt.binding(
                                function()
                                {
                                    return (((applicationLoader.item != null) && (applicationLoader.item.internalServices !== undefined)) ? applicationLoader.item.internalServices : [])
                                })
                    serviceDiscovery.updateServices()
                }
            }
        }

        state: {
            if (serviceDiscovery.networkOpen)
            {
                if (localMode && discoveryPage.instanceSelected)
                {
                    return "loaded"
                }
                else if (applicationConfig.ready)
                {
                    return ((applicationLoader.active) ?"loaded":"config")
                }

                return "discovery"
            }
            else
            {
                return "network"
            }
        }

        states: [
            State {
                name: "network"
                PropertyChanges { target: networkPage; opacity: 1.0; z: 1; enabled: true }
                PropertyChanges { target: discoveryPage; opacity: 0.0; z: 0; enabled: false }
                PropertyChanges { target: appPage; opacity: 0.0; z: 0; enabled: false }
                PropertyChanges { target: viewPage; opacity: 0.0; z: 0; enabled: false }
            },
            State {
                name: "discovery"
                PropertyChanges { target: discoveryPage; opacity: 1.0; z: 1; enabled: true }
                PropertyChanges { target: appPage; opacity: 0.0; z: 0; enabled: false }
                PropertyChanges { target: viewPage; opacity: 0.0; z: 0; enabled: false }
                PropertyChanges { target: networkPage; opacity: 0.0; z: 0; enabled: false }
            },
            State {
                name: "config"
                PropertyChanges { target: appPage; opacity: 1.0; z: 1; enabled: true }
                PropertyChanges { target: viewPage; opacity: 0.0; z: 0; enabled: false }
                PropertyChanges { target: discoveryPage; opacity: 0.0; z: 0; enabled: false }
                PropertyChanges { target: networkPage; opacity: 0.0; z: 0; enabled: false }
            },
            State {
                name: "loaded"
                PropertyChanges { target: appPage; opacity: 0.0; z: 0; enabled: false }
                PropertyChanges { target: viewPage; opacity: 1.0; z: 1; enabled: true }
                PropertyChanges { target: discoveryPage; opacity: 0.0; z: 0; enabled: false }
                PropertyChanges { target: networkPage; opacity: 0.0; z: 0; enabled: false }
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
                goBack()
                event.accepted = true
            }
        }
    }

    ApplicationConfig {
        id: applicationConfig
        ready: false
        uri: configService.uri
        filters: [ ApplicationConfigFilter { type: ApplicationConfigItem.QT5_QML } ]
    }

    ServiceDiscovery {
        id: serviceDiscovery
        regType: "machinekit"
        domain: "local"
        running: true
        filter: ServiceDiscoveryFilter {
            id: serviceDiscoveryFilter
            name: ""
        }

        serviceLists: [
            ServiceList {
                services: [
                    Service {
                        id: configService
                        type: "config"

                        filter: ServiceDiscoveryFilter { name: "Machinekit" }
                    }
                ]
            },
            ServiceList {
                id: applicationInternalServiceList
            },
            ServiceList {
                id: applicationServiceList
            }
        ]
    }
}
