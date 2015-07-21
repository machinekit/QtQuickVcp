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
import QtQuick.Controls 1.1
import QtQuick.Window 2.0
import Machinekit.HalRemote 1.0
import Machinekit.Service 1.0

/*!
    \qmltype HalApplicationWindow
    \inqmlmodule Machinekit.HalRemote.Controls
    \brief Provides a application window to simplify HAL application deployment.
    \ingroup halremotecontrols

    In order to create working HAL remote UI have to set \l title and \l name
    of the application window accordingly.

    \qml
    HalApplicationWindow {
        id: halApplicationWindow
        title: "My HAL application"
        name: "myhalapp"
    }
    \endqml
*/

Rectangle {
    /*! This property holds the title of the application window.
    */
    property string title: "HAL Application Template"

    /*! \qmlproperty string name

        This property holds the name of the HAL remote component that will
        contain all HAL pins created inside the window.
    */
    property alias name: remoteComponent.name

    /*! \qmlproperty int heartbeadPeriod

        This property holds the period time of the heartbeat timer in ms.

        The default value is \c{3000}.
    */
    property alias heartbeatPeriod: remoteComponent.heartbeatPeriod

    /*! \qmlproperty string halrcmdUri

        This property holds the halrcmd service uri. It needs to be set in
        case the internal services are not used.
    */
    property alias halrcmdUri: remoteComponent.halrcmdUri

    /*! \qmlproperty string halrcompUri

        This property holds the halrcomp service uri. It needs to be set in
        case the internal services are not used.
    */
    property alias halrcompUri: remoteComponent.halrcompUri

    /*! \qmlproperty bool ready

        This property holds wheter the application window is ready or not.
        Per default this property is set using the internal services
        if you want to overwrite the services set this property to \c true.
    */
    property bool ready: remoteComponent.ready && _ready

    /*! \qmlproperty list<Service> services

        This property holds the services used by the application.
    */
    property var services: []

    /*!
        \qmlproperty Item toolBar

        This property holds the toolbar \l Item.

        It can be set to any Item type, but is generally used with \l ToolBar.

        By default, this value is not set. When you set the toolbar item, it will
        be anchored automatically into the application window.
    */
    property Item toolBar

    /*!
        \qmlproperty Item statusBar

        This property holds the status bar \l Item.

        It can be set to any Item type, but is generally used with \l StatusBar.

        By default, this value is not set. When you set the status bar item, it
        will be anchored automatically into the application window.
    */
    property Item statusBar

    /*!
        \qmlproperty MenuBar menuBar

        This property holds the \l MenuBar.

        By default, this value is not set.
    */
    property MenuBar menuBar

    /*! \internal */
    property var _requiredServices: {
        var required = []
        var newReady = true
        for (var i = 0; i < services.length; ++i) {
            if (services[i].required) {
                required.push(services[i])
                newReady = newReady && services[i].ready
                services[i].onReadyChanged.connect(_evaluateReady)
            }
        }

        _ready = newReady  // if no required service we are ready

        return required
    }

    /*! \internal */
    property bool _ready: false

    /*! \internal */
    function _evaluateReady() {
        for (var i = 0; i < _requiredServices.length; ++i) {
            if (!_requiredServices[i].ready) {
                _ready = false
                return
            }
        }

        _ready = true
        return
    }

    /*! \internal */
    function _recurseObjects(objects, name)
    {
        var list = []

        if (objects !== undefined) {
            for (var i = 0; i < objects.length; ++i)
            {
                if (objects[i].objectName === name) {
                    list.push(objects[i])
                }
                var nestedList = _recurseObjects(objects[i].data, name)
                if (nestedList.length > 0) {
                    list = list.concat(nestedList)
                }
            }
        }

        return list;
    }

    /*! \qmlsignal disconnect

        Disconnects the service window if connected.
      */
    signal disconnect()

    Component.onCompleted: {
        var list = main.services
        var nestedList = _recurseObjects(main.data, "Service")
        if (nestedList.length > 0) {
            list = list.concat(nestedList)
        }
        main.services = list
    }

    id: main

    width: 500
    height: 800
    color: systemPalette.window

    SystemPalette {
        id: systemPalette;
        colorGroup: enabled ? SystemPalette.Active : SystemPalette.Disabled
    }

    Label {
        id: dummyText
    }

    Rectangle {
        id: discoveryPage

        anchors.fill: parent
        visible: false
        z: 100
        color: systemPalette.window


        Label {
            id: connectingLabel

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: connectingIndicator.top
            anchors.bottomMargin: Screen.pixelDensity
            font.pointSize: dummyText.font.pointSize * 1.3
            text: (remoteComponent.connectionState === HalRemoteComponent.Disconnected)
                ? qsTr("Waiting for services to appear...")
                : qsTr("Connecting...")
        }

        BusyIndicator {
            id: connectingIndicator

            anchors.centerIn: parent
            running: true
            height: parent.height * 0.15
            width: height
        }

        Column {
            id: serviceCheckColumn
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: connectingIndicator.bottom
            anchors.topMargin: Screen.pixelDensity

            Repeater {
                model: main._requiredServices.length

                CheckBox {
                    id: checkBox
                    text: _requiredServices[index].type + qsTr(" service")
                    checked: _requiredServices[index].ready
                }
            }
        }

        Label {
            id: errorLabel

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: Screen.pixelDensity * 2
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.pointSize: dummyText.font.pointSize * 1.1
            text: {
                var headerText

                switch (remoteComponent.error)
                {
                case HalRemoteComponent.BindError: headerText = qsTr("Bind rejected error:"); break;
                case HalRemoteComponent.PinChange: headerText = qsTr("Pin change rejected error:"); break;
                case HalRemoteComponent.CommandError: headerText = qsTr("Command error:"); break;
                case HalRemoteComponent.SocketError: headerText = qsTr("Socket error:"); break;
                default: headerText = qsTr("No error")
                }

                return headerText + "\n" + remoteComponent.errorString
            }
        }
    }

    Service {
        id: halrcompService
        type: "halrcomp"
        required: true
    }

    Service {
        id: halrcmdService
        type: "halrcmd"
        required: true
    }

    HalRemoteComponent {
        id: remoteComponent

        name: main.name
        halrcmdUri: halrcmdService.uri
        halrcompUri: halrcompService.uri
        heartbeatPeriod: 3000
        ready: (halrcompService.ready && halrcmdService.ready) || (remoteComponent.state === HalRemoteComponent.Connected)
        containerItem: parent
    }

    /* This timer is a workaround to make the discoveryPage invisible in QML designer */
    Timer {
        interval: 10
        repeat: false
        running: true
        onTriggered: {
            discoveryPage.visible = true
        }
    }

    state: {
        switch (remoteComponent.connectionState) {
        case HalRemoteComponent.Connected:
            if (_ready)
                return "connected"
            else
                return "disconnected"
        case HalRemoteComponent.Error:
                return "error"
        case HalRemoteComponent.Timeout:
            return "timeout"
        default:
            return "disconnected"
        }
    }

    states: [
        State {
            name: "disconnected"
            PropertyChanges { target: discoveryPage; opacity: 1.0; enabled: true }
            PropertyChanges { target: connectingLabel; visible: true }
            PropertyChanges { target: connectingIndicator; visible: true }
            PropertyChanges { target: serviceCheckColumn; visible: true }
            PropertyChanges { target: errorLabel; visible: false }
        },
        State {
            name: "error"
            PropertyChanges { target: discoveryPage; opacity: 1.0; enabled: true }
            PropertyChanges { target: connectingLabel; visible: false }
            PropertyChanges { target: connectingIndicator; visible: false }
            PropertyChanges { target: serviceCheckColumn; visible: false }
            PropertyChanges { target: errorLabel; visible: true }
        },
        State {
            name: "timeout"
            PropertyChanges { target: discoveryPage; opacity: 0.0; enabled: false }
            PropertyChanges { target: main; enabled: false }
        },
        State {
            name: "connected"
            PropertyChanges { target: discoveryPage; opacity: 0.0; enabled: false }
            PropertyChanges { target: main; enabled: true }
        }
    ]

    transitions: Transition {
            PropertyAnimation { duration: 500; properties: "opacity"; easing.type: Easing.InCubic}
        }
}
