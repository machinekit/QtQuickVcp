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
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import Machinekit.Controls 1.0
import Machinekit.Service 1.0
import Machinekit.Application 1.0

/*!
    \qmltype ConnectionWindow
    \inqmlmodule Machinekit.Application.Controls
    \brief Provides an all in one container item to load and display Machinekit UIs.
    \ingroup applicationcontrols

    This component provides an all in container item to load and display Machinekit UIs.
    It combines service discovery and instance selection for local as well as remote
    deployed \l{HalApplicationWindow}s and \l{ServiceWindow}s.

    The following example demonstrates how to use the ConnectionWindow as generic Machinekit
    client.

    \qml
    ConnectionWindow {
        id: connectionWindow

        anchors.fill: parent
        autoSelectInstance: false
        remoteVisible: true
        instanceFilter: ServiceDiscoveryFilter{ name: "" }
    }
    \endqml

    It can also be used to load local UIs e.g. for development purposes.

    \qml
    ConnectionWindow {
        id: connectionWindow

        anchors.fill: parent
        autoSelectInstance: false
        applications: [
            ApplicationDescription {
                sourceDir: "qrc:/AppDiscover.ServiceDisplay"
            }
        ]
    }
    \endqml

    Use a combination of \l instanceFilter and \l autoSelectInstance to automatically
    connect to a specific instance. This can be especially usefull during development
    of new UIs.

    \qml
    ConnectionWindow {
        id: connectionWindow

        anchors.fill: parent
        autoSelectInstance: true
        instanceFilter: ServiceDiscoveryFilter{ name: "Development" }
        applications: [
            ApplicationDescription {
                sourceDir: "qrc:/AppDiscover.ServiceDisplay"
            }
        ]
    \endqml

    It is also possible to use a combination of both local and remote \l{mode}.
    This can be used to ship tools within the client application.

    \qml
    ConnectionWindow {
        id: connectionWindow

        anchors.fill: parent
        autoSelectInstance: false
        remoteVisible: true
        localVisible: true
        mode: "remote"
        applications: [
            ApplicationDescription {
                sourceDir: "qrc:/AppDiscover.ServiceDisplay"
            }
        ]
        }
    \endqml
*/

Rectangle {
    /*! This property holds whether an instance should be automatically selected or not.
    */
    property bool autoSelectInstance: false

    /*! This property holds whether an application should be automatically selected or not.
    */
    property bool autoSelectApplication: !remoteVisible && (applications.length === 1)

    /*! This property holds whether the local application selection should be visible or not.
    */
    property bool localVisible: applications.length > 0

    /*! This property holds whether the remote application selection should be visible or not.
    */
    property bool remoteVisible: !localVisible

    /*! This property holds the currently selected application mode. Legal value are \c{"local"} and \c{"remote"}.
    */
    property string mode: localVisible ? "local" : "remote"

    /*! \qmlproperty ServiceDiscoveryFilter instanceFilter

        This property can be used to filter all available instances for a specific name or TXT record.
    */
    property alias instanceFilter: configService.filter

    /*! \qmlproperty ApplicationConfigFilter applicationFilters

        This property can be used to filter all available application configurations for type, name or description.
    */
    property alias applicationFilter: applicationConfig.filter

    /*! \qmlproperty list<NameServer> nameServers

        This property holds a list of name servers. Name servers have to specified
        in case unicast DNS is used as \l lookupMode . If this list is empty the
        default name servers of the system will be used.

    */
    property alias nameServers: serviceDiscovery.nameServers

    /*! \qmlproperty enumeration lookupMode

        This property holds the method that should be used for discovering services.

        \list
        \li ServiceDiscovery.MulticastDNS - Multicast DNS (mDNS) is used to discovery services. (default)
        \li ServiceDiscovery.UnicastDNS - Unicast DNS is used to discover services. It is necessary to specify \l nameServers .
        \endlist
    */
    property alias lookupMode: serviceDiscovery.lookupMode

    /*! This property holds wheter the service discovery configuration should be stored or not
    */
    property bool autoSaveConfiguration: true

    /*! This property holds the path to a configuration file.
        If this property is not set the configuration file path is automatically generated
        if \l autoSaveConfiguration is \c{true}
    */
    property string configurationFilePath: ""

    /*! This property holds the title of the window.
    */
    readonly property string title: (applicationLoader.active && (applicationLoader.item != null))
                           ? ((applicationLoader.item.title !== undefined) ? applicationLoader.item.title : "")
                           : defaultTitle

    /*! This property holds the title of the window displayed if no application is loaded.
    */
    property string defaultTitle: qsTr("Machinekit")

    /*! This property holds the list of local applications.
    */
    property list<ApplicationDescription> applications

    /*!
        \qmlproperty Item toolBar

        This property holds the toolbar \l Item.

        This property is set by the connection window.
    */
    property Item toolBar

    /*!
        \qmlproperty Item statusBar

        This property holds the status bar \l Item.

        This property is set by the connection window.
    */
    property Item statusBar

    /*!
        \qmlproperty MenuBar menuBar

        This property holds the \l MenuBar.

        This property is set by the connection window.
    */
    property MenuBar menuBar

    id: mainWindow

    color: systemPalette.window
    width: 500
    height: 700

    /*! \internal */
    function selectInstance(index)
    {
        if (!(remoteVisible || localVisible))
            return

        if ((configService.items[index].uuid !== "")
                && (!remoteVisible || configService.items[index].uri !== ""))
        {
            var x = applicationConfig   // for some reason the appConfig variable goes away after settings the filter
            serviceDiscoveryFilter.txtRecords = ["uuid=" + configService.items[index].uuid]
            serviceDiscovery.updateFilter()
            if (remoteVisible)
                x.ready = true
            d.instanceSelected = true

            if ((mainWindow.mode == "local") && mainWindow.autoSelectApplication)
                selectApplication(0)
        }
        else
        {
            console.log("selecting instance failed: check uri and uuid")
            setError(qsTr("Instance Error:"), qsTr("Check uri and uuid"))
        }
    }

    /*! \internal */
    function selectApplication(index)
    {
        if (mode == "local")
            d.applicationSource = applications[index].mainFile
        else
            applicationConfig.selectConfig(applicationConfig.configs[index].name)
    }

    /*! \internal */
    function goBack()
    {
        if (mainWindow.state == "discovery")
        {
            Qt.quit()
        }
        else if (mainWindow.state == "config")
        {
            if (autoSelectInstance == false)
            {
                applicationConfig.ready = false
                serviceDiscoveryFilter.txtRecords = []
                serviceDiscovery.updateFilter()
                d.instanceSelected = false
            }
            else
            {
                Qt.quit()
            }
        }
        else if ((mainWindow.state == "loaded") || (mainWindow.state == "loading"))
        {
            if ((autoSelectApplication) && (autoSelectInstance))
                Qt.quit()

            if (d.applicationSource == "")    // remote application
                applicationConfig.unselectConfig()
            else
                d.applicationSource = ""

            applicationServiceList.services = []
            serviceDiscovery.updateServices()

            if (autoSelectApplication)  // go back to discovery page
            {
                serviceDiscoveryFilter.txtRecords = []
                serviceDiscovery.updateFilter()
                d.instanceSelected = false
            }
        }
        else if (mainWindow.state == "error")
        {
            clearError()
            goBack()
        }
    }

    /*! \internal */
    function setError(errorType, errorText)
    {
        d.errorType = errorType
        d.errorText = errorText
        d.errorActive = true
    }

    /*! \internal */
    function clearError()
    {
        d.errorActive = false
    }

    /*! \internal */
    function loadSettings() {
        var manualConfig = false
        if (configurationFilePath !== "") {
            sdSettings.filePath = configurationFilePath
            manualConfig = true
        }
        else if (!autoSaveConfiguration) {
            return
        }

        sdSettings.load()
        sdSettings.setValue("nameServers", serviceDiscovery.nameServers, false)
        sdSettings.setValue("lookupMode", serviceDiscovery.lookupMode, false)
        sdSettings.setValue("mode", mode, false)

        // add stored name servers
        var nameServers = sdSettings.values.nameServers
        var currentNameServers = serviceDiscovery.nameServers
        for (var i = 0; i < nameServers.length; ++i)
        {
            var found = false
            for (var j = 0; j < currentNameServers.length; ++j)     // avoid duplicates
            {
                if ((nameServers[i].hostName === currentNameServers[j].hostName)
                        && (nameServers[i].port === currentNameServers[j].port))
                {
                    found = true
                    break
                }
            }

            if (!found && (serviceDiscovery.nameServers.length < 3)) // limit to 3 name servers => TODO
            {
                var nameServerObject = nameServerComponent.createObject(mainWindow, {})
                nameServerObject.hostName = nameServers[i].hostName
                nameServerObject.port = nameServers[i].port
                serviceDiscovery.addNameServer(nameServerObject)
            }
        }

        serviceDiscovery.lookupMode = sdSettings.values.lookupMode
        mode = sdSettings.values.mode

        if (manualConfig) {
            sdSettings.setValue("autoSaveConfiguration", false, false)
            sdSettings.setValue("autoSelectInstance", autoSelectInstance, false)
            sdSettings.setValue("autoSelectApplication", autoSelectApplication, false)
            sdSettings.setValue("instanceFilter", serviceDiscovery.filter, false)
            sdSettings.setValue("applicationFilter", applicationConfig.filter, false)

            autoSaveConfiguration = sdSettings.values.autoSaveConfiguration
            autoSelectInstance = sdSettings.values.autoSelectInstance
            autoSelectApplication = sdSettings.values.autoSelectApplication
            for (var key in sdSettings.values.instanceFilter) {
                serviceDiscovery.filter[key] = sdSettings.values.instanceFilter[key]
            }
            for (key in sdSettings.values.applicationFilter) {
                applicationConfig.filter[key] = sdSettings.values.applicationFilter[key]
            }
        }

        sdSettings.initialized = true
    }

    /*! \internal */
    function saveSettings() {
        if (!sdSettings.initialized || !autoSaveConfiguration) {
            return
        }

        var nameServerList = []
        var nameServer

        for (var i = 0; i < serviceDiscovery.nameServers.length; ++i) {
            nameServer = {"hostName": "", "port": 0}
            nameServer.hostName = serviceDiscovery.nameServers[i].hostName
            nameServer.port = serviceDiscovery.nameServers[i].port
            if (nameServer.hostName !== "") {
                nameServerList.push(nameServer)
            }
        }

        sdSettings.setValue("nameServers", nameServerList)
        sdSettings.setValue("lookupMode", serviceDiscovery.lookupMode)
        sdSettings.setValue("mode", mode)
        sdSettings.save()
    }

    Component.onCompleted: {
        loadSettings()
    }

    onModeChanged: saveSettings()
    onLookupModeChanged: saveSettings()
    onNameServersChanged: saveSettings()

    // Capture the Android Back key and backspace key
    // on the desktop tp go back in the application
    // focus needs to be true to capture key events
    focus: true
    Keys.onReleased: {
        if ((event.key === Qt.Key_Back) ||
                (event.key === Qt.Key_Backspace)) {
            if (!mainWindow.activeFocus)
                return;
            goBack()
            event.accepted = true
        }
    }

    /* Global Variables */
    QtObject {
        id: d
        property bool instanceSelected: false   // indicates whether an instance was selected or not
        property bool errorActive: false        // indicates whether an error is active or not
        property string errorType: ""           // a string representing the type of the current error
        property string errorText: ""           // a string representing the text of the current error
        property string applicationSource: ""   // indicates that a local app was selected
    }

    SystemPalette {
        id: systemPalette;
        colorGroup: enabled ? SystemPalette.Active : SystemPalette.Disabled
    }

    Label {
        id: dummyText
        visible: false
    }

    Button {
        id: dummyButon
        visible: false
    }

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
            font.pointSize: dummyText.font.pointSize * 1.1
            text: qsTr("Warning!<br>No network connection found, service discovery unavailable. Please check your network connection.")
        }
    }

    Item {
        id: discoveryPage

        anchors.fill: parent

        Component {
            id: instanceListView

            ListView {
                spacing: Screen.logicalPixelDensity*3
                clip: true

                model: configService.items
                delegate: Button {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: dummyButon.height * 3

                    Label {
                        id: titleText2

                        anchors.fill: parent
                        font.pointSize: dummyText.font.pointSize*1.3
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: name
                        elide: Text.ElideRight
                    }

                    onClicked: selectInstance(index)
                }

                onCountChanged: {
                    if ((mainWindow.state == "discovery") && (autoSelectInstance == true) && (count > 0))
                    {
                        selectInstance(0)
                    }
                }

                BusyIndicator {
                    anchors.centerIn: parent
                    running: true
                    visible: configService.items.length === 0
                    height: parent.height * 0.15
                    width: height
                }
            }
        }

        SlideView {
            id: discoveryView
            anchors.fill: parent

            onCurrentIndexChanged: {
                if (currentIndex == 0)
                    serviceDiscovery.lookupMode = ServiceDiscovery.MulticastDNS
                else
                    serviceDiscovery.lookupMode = ServiceDiscovery.UnicastDNS
            }

            Binding {
                target: discoveryView; property: "currentIndex";
                value: (serviceDiscovery.lookupMode == ServiceDiscovery.MulticastDNS) ? 0 : 1
            }

            SlidePage {
                title: qsTr("Multicast")

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Screen.logicalPixelDensity*3
                    spacing: Screen.logicalPixelDensity*3

                    Label {
                        id: pageTitleText2

                        Layout.fillWidth: true
                        text: qsTr("Available Instances:")
                        font.pointSize: dummyText.font.pointSize * 1.3
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                    }

                    Loader {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        sourceComponent: instanceListView
                        active: true
                    }
                }
            }

            SlidePage {
                id: unicastPage
                title: qsTr("Unicast")

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Screen.logicalPixelDensity*3
                    spacing: Screen.logicalPixelDensity*3

                    Label {
                        Layout.fillWidth: true
                        text: qsTr("Available Instances:")
                        font.pointSize: dummyText.font.pointSize * 1.3
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                    }

                    Loader {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        sourceComponent: instanceListView
                        active: true
                    }

                    Label {
                        Layout.fillWidth: true
                        text: qsTr("DNS Servers:")
                        font.pointSize: dummyText.font.pointSize * 1.3
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                    }

                    ListView {
                        id: dnsServerView
                        Layout.fillWidth: true
                        Layout.preferredHeight: dummyButon.height * 1.5 * model.length + Screen.logicalPixelDensity * 1.5 * Math.max(model.length-1, 0)
                        spacing: Screen.logicalPixelDensity*1.5

                        model: serviceDiscovery.nameServers

                        delegate: RowLayout {
                                    id: viewItem
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    height: dummyButon.height * 1.5

                                    Label {
                                        text: qsTr("Server ") + (index + 1) + ":"
                                        font.pointSize: dummyText.font.pointSize * 1.2
                                    }

                                    TextField {
                                        id: dnsServerTextField
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        font.pointSize: dummyText.font.pointSize * 1.2
                                        onEditingFinished: {
                                            dnsServerView.model[index].hostName = text
                                            serviceDiscovery.updateNameServers()
                                            serviceDiscovery.updateConfig() // nameServers have changed but are not updated

                                            mainWindow.forceActiveFocus()   // remove the focus
                                        }

                                        Binding {
                                            target: dnsServerTextField;
                                            property: "text";
                                            value: (dnsServerView.model[index] !== null) ? dnsServerView.model[index].hostName : ""
                                        }
                                    }

                                    Button {
                                        Layout.fillHeight: true
                                        text: (dnsServerTextField.text !== "") ? "+" : "-"
                                        visible: (index === (dnsServerView.model.length - 1)) && (index < 2)   // last item, limited to 3 items due to bug => TODO
                                        onClicked: {
                                            mainWindow.forceActiveFocus()   // accept changes on text edit

                                            if (dnsServerTextField.text && dnsServerView.model[index].hostName)
                                            {
                                                var nameServerObject = nameServerComponent.createObject(mainWindow, {})
                                                serviceDiscovery.addNameServer(nameServerObject)
                                            }
                                        }

                                        Label {
                                            anchors.fill: parent
                                            font.pointSize: dummyText.font.pointSize*1.2
                                            font.bold: true
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                            text: "+"
                                        }
                                    }
                                }
                    }

                    Button {
                        Layout.fillWidth: true
                        Layout.preferredHeight: dummyButon.height * 1.5
                        visible: dnsServerView.model.length === 0
                        onClicked: {
                            serviceDiscovery.addNameServer(nameServerComponent.createObject(mainWindow, {}))
                        }

                        Label {
                            anchors.fill: parent
                            font.pointSize: dummyText.font.pointSize*1.2
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: "+"
                        }
                    }
                }
            }
        }
    }

    Item {
        id: appPage

        anchors.fill: parent

        Component {
            id: localRemoteContent

            ColumnLayout {
                spacing: Screen.logicalPixelDensity*3

                Label {
                    id: pageTitleText

                    Layout.fillWidth: true
                    text: configService.name
                    font.pointSize: dummyText.font.pointSize * 1.3
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                }

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: Screen.logicalPixelDensity*3
                    clip: true

                    model: (mode == "local") ? applications : applicationConfig.configs
                    delegate: Button {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: dummyButon.height * 3

                        ColumnLayout {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter

                            Label {
                                id: titleText

                                Layout.fillWidth: true
                                font.pointSize: dummyText.font.pointSize*1.3
                                font.bold: true
                                text: name
                                horizontalAlignment: Text.AlignHCenter
                                elide: Text.ElideRight
                            }
                            Label {
                                id: descriptionText

                                Layout.fillWidth: true
                                text: description
                                color: systemPalette.dark
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WordWrap
                            }
                        }

                        onClicked: selectApplication(index)
                    }

                    onCountChanged: {
                        if ((mainWindow.state == "config") && (autoSelectApplication == true) && (count > 0))
                        {
                            selectApplication(0)
                        }
                    }
                }
            }
        }

        SlideView {
            id: appView
            anchors.fill: parent

            onCurrentIndexChanged: {
                if (currentIndex == 0)
                    mainWindow.mode = "remote"
                else
                    mainWindow.mode = "local"
            }

            Binding {
                target: appView; property: "currentIndex";
                value: ((mainWindow.mode == "remote") ? 0 : 1)
            }

            SlidePage {
                anchors.fill: parent
                anchors.margins: Screen.logicalPixelDensity*3
                title: qsTr("Remote")
                visible: mainWindow.remoteVisible

                Loader {
                    anchors.fill: parent
                    sourceComponent: localRemoteContent
                    active: true
                }
            }

            SlidePage {
                anchors.fill: parent
                anchors.margins: Screen.logicalPixelDensity*3
                title: qsTr("Local")
                visible: mainWindow.localVisible

                Loader {
                    anchors.fill: parent
                    sourceComponent: localRemoteContent
                    active: true
                }
            }
        }
    }

    Item {
        id: viewPage

        anchors.fill: parent

        Loader {
            id: applicationLoader

            anchors.fill: parent
            active: (d.applicationSource != "") ? true : applicationConfig.selectedConfig.loaded
            source: (d.applicationSource != "") ? d.applicationSource : applicationConfig.selectedConfig.mainFile

            onSourceChanged: {
                console.log("Source changed: " + source + " " + active)
            }

            onStatusChanged: {
                if (applicationLoader.status === Loader.Error)
                {
                    setError(qsTr("QML Error:"), "Loading QML file failed")
                }
            }

            onLoaded: {
                console.log("Window " + applicationLoader.item.title + " loaded")
                applicationServiceList.services = Qt.binding(
                            function()
                            {
                                return (((applicationLoader.item != null) && (applicationLoader.item.services !== undefined)) ? applicationLoader.item.services : [])
                            })
                mainWindow.toolBar = Qt.binding(
                            function()
                            {
                                return ((applicationLoader.item != null) ? applicationLoader.item.toolBar : null)
                            })
                mainWindow.statusBar = Qt.binding(
                            function()
                            {
                                return ((applicationLoader.item != null) ? applicationLoader.item.statusBar : null)
                            })
                mainWindow.menuBar = Qt.binding(
                            function()
                            {
                                return ((applicationLoader.item != null) ? applicationLoader.item.menuBar : null)
                            })
                serviceDiscovery.updateServices()
                applicationLoader.item.onServicesChanged.connect(serviceDiscovery.updateServices)
            }
        }
    }

    Item {
        id: loadingPage

        anchors.fill: parent

        Label {
            id: connectingLabel

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: connectingIndicator.top
            anchors.bottomMargin: Screen.logicalPixelDensity
            font.pointSize: dummyText.font.pointSize * 1.3
            text: qsTr("Loading ") + applicationConfig.selectedConfig.name + "..."
        }

        BusyIndicator {
            id: connectingIndicator

            anchors.centerIn: parent
            running: true
            height: (parent.height > parent.width) ? parent.height * 0.10 : parent.width * 0.10
            width: height
        }
    }

    Item {
        id: errorPage

        anchors.fill: parent

        Label {
            id: errorLabel

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: Screen.logicalPixelDensity * 2
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.pointSize: dummyText.font.pointSize * 1.1
            text: d.errorType + "\n" + d.errorText
        }
    }

    ApplicationConfig {
        id: applicationConfig

        ready: false
        configUri: configService.uri
        onConnectionStateChanged: {
            if (applicationConfig.connectionState === ApplicationConfig.Error)
            {
                setError(qsTr("Application Config Error:"), applicationConfig.errorString)
            }
        }
    }

    ServiceDiscovery {
        id: serviceDiscovery

        serviceType: "machinekit"
        domain:      "local"
        running:     true
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
                        filter: ServiceDiscoveryFilter { name: "" }
                    }
                ]
            },
            ServiceList {
                id: applicationServiceList
            }
        ]
    }

    LocalSettings {
        property bool initialized: false

        id: sdSettings

        application: "machinekit"
        name: "service-discovery"
    }

    Component {
        id: nameServerComponent
        NameServer { }
    }

    state: {
        if (serviceDiscovery.networkReady)
        {
            if (d.errorActive)
            {
                return "error"
            }
            else if (applicationLoader.active)
            {
                return "loaded"
            }
            else if (applicationConfig.selectedConfig.loading)
            {
                return "loading"
            }
            else if (d.instanceSelected)
            {
                return "config"
            }
            else {
                return "discovery"
            }
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
            PropertyChanges { target: errorPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: loadingPage; opacity: 0.0; z: 0; enabled: false }
        },
        State {
            name: "discovery"
            PropertyChanges { target: discoveryPage; opacity: 1.0; z: 1; enabled: true }
            PropertyChanges { target: appPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: viewPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: networkPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: errorPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: loadingPage; opacity: 0.0; z: 0; enabled: false }
        },
        State {
            name: "config"
            PropertyChanges { target: appPage; opacity: 1.0; z: 1; enabled: true }
            PropertyChanges { target: viewPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: discoveryPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: networkPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: errorPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: loadingPage; opacity: 0.0; z: 0; enabled: false }
        },
        State {
            name: "loading"
            PropertyChanges { target: appPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: viewPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: discoveryPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: networkPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: errorPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: loadingPage; opacity: 1.0; z: 1; enabled: true }
        },
        State {
            name: "loaded"
            PropertyChanges { target: appPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: viewPage; opacity: 1.0; z: 1; enabled: true }
            PropertyChanges { target: discoveryPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: networkPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: errorPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: loadingPage; opacity: 0.0; z: 0; enabled: false }
        },
        State {
            name: "error"
            PropertyChanges { target: appPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: viewPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: discoveryPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: networkPage; opacity: 0.0; z: 0; enabled: false }
            PropertyChanges { target: errorPage; opacity: 1.0; z: 1; enabled: true }
            PropertyChanges { target: loadingPage; opacity: 0.0; z: 0; enabled: false }
        }
    ]

    transitions: Transition {
            PropertyAnimation { duration: 500; properties: "opacity"; easing.type: Easing.InCubic }
        }
}
