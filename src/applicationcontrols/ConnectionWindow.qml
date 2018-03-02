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
import Machinekit.Application.Controls.Private 1.0

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
    property string mode: (localVisible && !remoteVisible) ? "local" : "remote"

    /*! \qmlproperty ServiceDiscoveryFilter instanceFilter

        This property can be used to filter all available instances for a specific name or TXT record.
    */
    property alias instanceFilter: launcherService.filter

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
    readonly property string title: (appPage.active ? appPage.title : defaultTitle) + d.instanceText

    /*! This property holds the title of the window displayed if no application is loaded.
    */
    property string defaultTitle: "Machinekit"

    /*! This property holds the list of local applications.
    */
    default property list<ApplicationDescription> applications

    /*!
        \qmlproperty Item toolBar

        This property holds the toolbar \l Item.

        This property is set by the connection window.
    */
    property Item toolBar: appPage.toolBar

    /*!
        \qmlproperty Item statusBar

        This property holds the status bar \l Item.

        This property is set by the connection window.
    */
    property Item statusBar: appPage.statusBar

    /*!
        \qmlproperty MenuBar menuBar

        This property holds the \l MenuBar.

        This property is set by the connection window.
    */
    property MenuBar menuBar: appPage.menuBar

    id: mainWindow

    color: systemPalette.window
    width: 500
    height: 700

    /*! \internal */
    function selectInstance(uuid)
    {
        if (!(remoteVisible || localVisible)) {
            return;
        }

        if (uuid !== "")
        {
            serviceDiscoveryFilter.txtRecords = ["uuid=" + uuid];
            serviceDiscovery.updateFilter();
            d.instanceSelected = true;
        }
        else
        {
            console.log("selecting instance failed: check uri and uuid");
            setError(qsTr("Instance Error:"), qsTr("Check uri and uuid"));
        }
    }

    /*! \internal */
    function selectLauncher(index)
    {
        var x = d; //goes away
        x.freshStart = false; // not fresh anymore
        x.holdLauncher = false; // we have started a new instance so we don't hold back
    }

    /*! \internal */
    function selectApplication(index)
    {
        if (mode === "local") {
            var application = applications[index];
            applicationTranslator.localName = application.name;
            applicationTranslator.localPath = application.translationsPath;
            d.applicationSource = application.mainFile;
        }
        else {
            applicationConfig.selectConfig(applicationConfig.configs[index].name);
        }
    }

    /*! \internal */
    function goBack(shutdown)
    {
        if (mainWindow.state === "instance")
        {
            Qt.quit();
        }
        else if (mainWindow.state === "launcher")
        {
            if (autoSelectInstance === false)
            {
                serviceDiscoveryFilter.txtRecords = [];
                serviceDiscovery.updateFilter();
                d.instanceSelected = false;
            }
            else
            {
                Qt.quit();
            }
        }
        else if ((mainWindow.state === "config") || (mainWindow.state === "launcher-selected"))
        {
            d.holdLauncher = true;
        }
        else if ((mainWindow.state === "app-loaded") || (mainWindow.state === "app-loading"))
        {
            if ((autoSelectApplication) && (autoSelectInstance)) {
                Qt.quit();
            }

            if (d.applicationSource === "") {   // remote application
                applicationConfig.unselectConfig();
            }
            else {
                d.applicationSource = "";
            }

            applicationServiceList.services = [];
            serviceDiscovery.updateServices();

            if (autoSelectApplication || shutdown) {
                d.holdLauncher = true;
            }

            if (autoSelectInstance)  // go back to discovery page
            {
                serviceDiscoveryFilter.txtRecords = [];
                serviceDiscovery.updateFilter();
                d.instanceSelected = false;
            }
        }
        else if (mainWindow.state === "error")
        {
            clearError();
            goBack();
        }
    }

    /*! \internal */
    function _evaluateState() {
        if (d.errorActive) {
            return "error";
        }
        else if (appPage.status === Loader.Ready) {
            return "app-loaded";
        }
        else if (applicationConfig.selectedConfig.loading
                 || (appPage.active && (appPage.status === Loader.Loading)))
        {
            return "app-loading";
        }
        else if (d.instanceSelected)
        {
            if (configService.ready && !d.holdLauncher) {
                return "config";
            }
            else if (d.holdLauncher || d.freshStart) {
                return "launcher";
            }
            else {
                return "launcher-selected";
            }
        }
        else {
            return "instance";
        }
        }

    /*! \internal */
    function setError(errorType, errorText)
    {
        d.errorType = errorType;
        d.errorText = errorText;
        d.errorActive = true;
    }

    /*! \internal */
    function clearError()
    {
        d.errorActive = false;
    }

    /*! \internal */
    function loadSettings() {
        var manualConfig = false;
        if (configurationFilePath !== "") {
            sdSettings.filePath = configurationFilePath;
            manualConfig = true;
        }
        else if (!autoSaveConfiguration) {
            return;
        }

        sdSettings.load();
        sdSettings.setValue("nameServers", serviceDiscovery.nameServers, false);
        sdSettings.setValue("lookupMode", serviceDiscovery.lookupMode, false);
        sdSettings.setValue("mode", mode, false);

        // add stored name servers
        var nameServers = sdSettings.values.nameServers;
        var currentNameServers = serviceDiscovery.nameServers;
        for (var i = 0; i < nameServers.length; ++i)
        {
            var found = false;
            for (var j = 0; j < currentNameServers.length; ++j)     // avoid duplicates
            {
                if ((nameServers[i].hostName === currentNameServers[j].hostName)
                        && (nameServers[i].port === currentNameServers[j].port))
                {
                    found = true;
                    break;
                }
            }

            if (!found && (serviceDiscovery.nameServers.length < 3)) // limit to 3 name servers => TODO
            {
                var nameServerObject = nameServerComponent.createObject(mainWindow, {});
                nameServerObject.hostName = nameServers[i].hostName;
                nameServerObject.port = nameServers[i].port;
                serviceDiscovery.addNameServer(nameServerObject);
            }
        }

        serviceDiscovery.lookupMode = sdSettings.values.lookupMode;
        if (localVisible && !remoteVisible) {
            mode = "local";
        }
        else if (remoteVisible && !localVisible) {
            mode = "remote";
        }
        else {
            mode = sdSettings.values.mode;
        }

        if (manualConfig) {
            sdSettings.setValue("autoSaveConfiguration", false, false);
            sdSettings.setValue("autoSelectInstance", autoSelectInstance, false);
            sdSettings.setValue("autoSelectApplication", autoSelectApplication, false);
            sdSettings.setValue("instanceFilter", serviceDiscovery.filter, false);
            sdSettings.setValue("applicationFilter", applicationConfig.filter, false);

            autoSaveConfiguration = sdSettings.values.autoSaveConfiguration;
            autoSelectInstance = sdSettings.values.autoSelectInstance;
            autoSelectApplication = sdSettings.values.autoSelectApplication;
            for (var key in sdSettings.values.instanceFilter) {
                serviceDiscovery.filter[key] = sdSettings.values.instanceFilter[key];
            }
            for (key in sdSettings.values.applicationFilter) {
                applicationConfig.filter[key] = sdSettings.values.applicationFilter[key];
            }
        }

        sdSettings.initialized = true;
    }

    /*! \internal */
    function saveSettings() {
        if (!sdSettings.initialized || !autoSaveConfiguration) {
            return;
        }

        var nameServerList = [];
        var nameServer;

        for (var i = 0; i < serviceDiscovery.nameServers.length; ++i) {
            nameServer = { "hostName": "", "port": 0 };
            nameServer.hostName = serviceDiscovery.nameServers[i].hostName;
            nameServer.port = serviceDiscovery.nameServers[i].port;
            if (nameServer.hostName !== "") {
                nameServerList.push(nameServer);
            }
        }

        sdSettings.setValue("nameServers", nameServerList);
        sdSettings.setValue("lookupMode", serviceDiscovery.lookupMode);
        sdSettings.setValue("mode", mode);
        sdSettings.save();
    }

    Component.onCompleted: {
        loadSettings();
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
            if (!mainWindow.activeFocus) {
                return;
            }
            goBack();
            event.accepted = true;
        }
        else {
            event.accepted = false;
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
        property bool holdLauncher: false       // when this property is true we do not automatically proceed to the config state
        property bool freshStart: true          // indicates if this session was freshly started
        property int selectedLauncher: 0        // index of the selected launcher
        readonly property string instanceText: instanceSelected ? " - " + launcherService.hostName + ' - ' + launcherService.hostAddress : ""
    }

    SystemPalette {
        id: systemPalette;
        colorGroup: enabled ? SystemPalette.Active : SystemPalette.Disabled
    }

    InstancePage {
        id: instancePage
        anchors.fill: parent

        autoSelectInstance: mainWindow.autoSelectInstance
        instances: launcherService.items
        serviceDiscovery: serviceDiscovery

        onInstanceSelected: mainWindow.selectInstance(uuid)
        onNameServersChanged: saveSettings()
    }

    LauncherPage {
        id: launcherPage
        anchors.fill: parent

        applicationLauncher: applicationLauncher
        configService: configService

        onGoBack: mainWindow.goBack()
        onLauncherSelected: mainWindow.selectLauncher(index)
        onSystemShutdown: applicationLauncher.shutdown()
    }

    SelectedPage {
        id: selectedPage
        anchors.fill: parent

        launcher: (launcherPage.selectedLauncher < applicationLauncher.launchers.length) ?
                   applicationLauncher.launchers[launcherPage.selectedLauncher] : undefined
        applicationLog: applicationLog
        onGoBack: mainWindow.goBack()
    }

    ConfigPage {
        id: configPage
        anchors.fill: parent

        instanceSelected: d.instanceSelected
        autoSelectApplication: mainWindow.autoSelectApplication
        localVisible: mainWindow.localVisible
        remoteVisible: mainWindow.remoteVisible
        configService: configService
        applications: mode === "local" ? mainWindow.applications : applicationConfig.configs

        onApplicationSelected: mainWindow.selectApplication(index)
        onGoBack: mainWindow.goBack()

        Binding { target: configPage; property: "mode"; value: mainWindow.mode }
        Binding { target: mainWindow; property: "mode"; value: configPage.mode }
    }

    AppPage {
        id: appPage
        anchors.fill: parent

        applicationSource: d.applicationSource
        applicationConfig: applicationConfig
        serviceDiscovery: serviceDiscovery

        onGoBack: mainWindow.goBack(shutdown)
        onServicesChanged: {
            applicationServiceList.services = services;
            serviceDiscovery.updateServices();
        }
    }

    LoadingPage {
        id: loadingPage
        anchors.fill: parent

        applicationConfig: applicationConfig

        onGoBack: mainWindow.goBack()
    }

    ErrorPage {
        id: errorPage
        anchors.fill: parent

        errorType: d.errorType
        errorText: d.errorText
    }

    ApplicationConfig {
        id: applicationConfig

        ready: ((mainWindow.state === "config") || (mainWindow.state === "app-loading"))
               && remoteVisible && configService.ready
        configUri: configService.uri
        onConnectionStateChanged: {
            if (applicationConfig.connectionState === ApplicationConfig.Error)
            {
                setError(qsTr("Application Config Error:"), applicationConfig.errorString);
            }
        }
    }

    ApplicationTranslator {
        property string localName: ""
        property string localPath: ""

        id: applicationTranslator
        applicationName: mainWindow.mode === "local" ? localName : applicationConfig.selectedConfig.name
        translationsPath: mainWindow.mode === "local" ? localPath : applicationConfig.selectedConfig.translationsPath
    }

    ApplicationLauncher {
        id: applicationLauncher
        ready: ((mainWindow.state === "launcher") || (mainWindow.state === "launcher-selected"))
               && launcherService.ready && launchercmdService.ready
        launcherUri: launcherService.uri
        launchercmdUri: launchercmdService.uri
        onConnectionStateChanged: {
            if (applicationLauncher.connectionState === ApplicationLauncher.Error) {
                setError(qsTr("Application Launcher Error:"), applicationLauncher.errorString);
            }
        }
    }

    ApplicationLog {
        id: applicationLog
        logLevel: ApplicationLog.Debug
        ready: ((mainWindow.state === "launcher") || (mainWindow.state === "launcher-selected"))
            && logService.ready
        logUri: logService.uri
    }

    ServiceDiscovery {
        id: serviceDiscovery

        running: true
        filter {
            id: serviceDiscoveryFilter
            name: ""
        }

        ServiceList {
            Service {
                id: configService
                type: "config"
            }
            Service {
                id: launcherService
                type: "launcher"
            }
            Service {
                id: launchercmdService
                type: "launchercmd"
            }
            Service {
                id: logService
                type: "log"
            }
        }

        ServiceList {
            id: applicationServiceList
            services: []
        }
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

    state: _evaluateState()

    states: [
        State {
            name: "instance"
            PropertyChanges { target: instancePage; visible: true }
            PropertyChanges { target: launcherPage; visible: false }
            PropertyChanges { target: selectedPage; visible: false }
            PropertyChanges { target: configPage; visible: false }
            PropertyChanges { target: appPage; visible: false }
            PropertyChanges { target: errorPage; visible: false}
            PropertyChanges { target: loadingPage; visible: false }
        },
        State {
            name: "launcher"
            PropertyChanges { target: instancePage; visible: false }
            PropertyChanges { target: launcherPage; visible: true }
            PropertyChanges { target: selectedPage; visible: false }
            PropertyChanges { target: configPage; visible: false }
            PropertyChanges { target: appPage; visible: false }
            PropertyChanges { target: errorPage; visible: false}
            PropertyChanges { target: loadingPage; visible: false }
        },
        State {
            name: "launcher-selected"
            PropertyChanges { target: instancePage; visible: false }
            PropertyChanges { target: launcherPage; visible: false }
            PropertyChanges { target: selectedPage; visible: true }
            PropertyChanges { target: configPage; visible: false }
            PropertyChanges { target: appPage; visible: false }
            PropertyChanges { target: errorPage; visible: false}
            PropertyChanges { target: loadingPage; visible: false }
        },
        State {
            name: "config"
            PropertyChanges { target: instancePage; visible: false }
            PropertyChanges { target: launcherPage; visible: false }
            PropertyChanges { target: selectedPage; visible: false }
            PropertyChanges { target: configPage; visible: true }
            PropertyChanges { target: appPage; visible: false }
            PropertyChanges { target: errorPage; visible: false}
            PropertyChanges { target: loadingPage; visible: false }
        },
        State {
            name: "app-loading"
            PropertyChanges { target: instancePage; visible: false }
            PropertyChanges { target: launcherPage; visible: false }
            PropertyChanges { target: selectedPage; visible: false }
            PropertyChanges { target: configPage; visible: false }
            PropertyChanges { target: appPage; visible: false }
            PropertyChanges { target: errorPage; visible: false}
            PropertyChanges { target: loadingPage; visible: true }
        },
        State {
            name: "app-loaded"
            PropertyChanges { target: instancePage; visible: false }
            PropertyChanges { target: launcherPage; visible: false }
            PropertyChanges { target: selectedPage; visible: false }
            PropertyChanges { target: configPage; visible: false }
            PropertyChanges { target: appPage; visible: true }
            PropertyChanges { target: errorPage; visible: false}
            PropertyChanges { target: loadingPage; visible: false }
        },
        State {
            name: "error"
            PropertyChanges { target: instancePage; visible: false }
            PropertyChanges { target: launcherPage; visible: false }
            PropertyChanges { target: selectedPage; visible: false }
            PropertyChanges { target: configPage; visible: false }
            PropertyChanges { target: appPage; visible: false }
            PropertyChanges { target: errorPage; visible: true}
            PropertyChanges { target: loadingPage; visible: false }
        }
    ]
}
