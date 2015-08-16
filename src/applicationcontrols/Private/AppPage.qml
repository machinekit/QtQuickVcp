import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

Loader {
    property string applicationSource: ""
    property var applicationConfig: undefined
    property var serviceDiscovery: undefined
    property string title: (active && (item != null))
                           ? ((item.title !== undefined) ? item.title : "") : ""
    property var services: (((item != null) && (item.services !== undefined)) ? item.services : [])
    property var toolBar: ((item != null) ? item.toolBar : null)
    property var statusBar: ((item != null) ? item.statusBar : null)
    property var menuBar: ((item != null) ? item.menuBar : null)

    signal goBack()

    id: applicationLoader
    width: 600
    height: 500

    active: (applicationSource != "") ? true : applicationConfig.selectedConfig.loaded
    source: (applicationSource != "") ? applicationSource : applicationConfig.selectedConfig.mainFile

    onSourceChanged: {
        console.log("Source changed: " + source + " " + active)
    }

    onStatusChanged: {
        if (status === Loader.Error)
        {
            setError(qsTr("QML Error:"), "Loading QML file failed")
        }
    }

    Connections {
        target: applicationLoader.item
        ignoreUnknownSignals: true
        onServicesChanged: serviceDiscovery.updateServices()
        onDisconnect: goBack()
    }
}

