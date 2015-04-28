import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

Item {
    property string applicationSource: ""
    property var applicationConfig: undefined
    property var serviceDiscovery: undefined
    property bool active: applicationLoader.active
    property string title: (applicationLoader.active && (applicationLoader.item != null))
                           ? ((applicationLoader.item.title !== undefined) ? applicationLoader.item.title : "") : ""
    property var services: []
    property var toolBar: null
    property var statusBar: null
    property var menuBar: null

    signal goBack()

    id: root
    width: 600
    height: 500

    Loader {
        id: applicationLoader

        anchors.fill: parent
        active: (applicationSource != "") ? true : applicationConfig.selectedConfig.loaded
        source: (applicationSource != "") ? applicationSource : applicationConfig.selectedConfig.mainFile

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
            root.services = Qt.binding(
                        function()
                        {
                            return (((applicationLoader.item != null) && (applicationLoader.item.services !== undefined)) ? applicationLoader.item.services : [])
                        })
            root.toolBar = Qt.binding(
                        function()
                        {
                            return ((applicationLoader.item != null) ? applicationLoader.item.toolBar : null)
                        })
            root.statusBar = Qt.binding(
                        function()
                        {
                            return ((applicationLoader.item != null) ? applicationLoader.item.statusBar : null)
                        })
            root.menuBar = Qt.binding(
                        function()
                        {
                            return ((applicationLoader.item != null) ? applicationLoader.item.menuBar : null)
                        })
            serviceDiscovery.updateServices()
            applicationLoader.item.onServicesChanged.connect(serviceDiscovery.updateServices)
            if (applicationLoader.item.onDisconnect) {
                applicationLoader.item.onDisconnect.connect(goBack)
            }
        }
    }
}

